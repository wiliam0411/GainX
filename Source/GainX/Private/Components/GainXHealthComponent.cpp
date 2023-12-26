// GainX, All Rights Reserved

#include "Components/GainXHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
#include "GainXGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UGainXHealthComponent::UGainXHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UGainXHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);
    return true;
}

bool UGainXHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UGainXHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    SetHealth(MaxHealth);
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UGainXHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UGainXHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UGainXHealthComponent::OnTakeRadialDamage);
    }
}

void UGainXHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}

void UGainXHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifaer(DamagedActor, BoneName);
    ApplyDamage(FinalDamage, InstigatedBy);
    UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage, *BoneName.ToString());
}

void UGainXHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
    const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
    ApplyDamage(Damage, InstigatedBy);
}

void UGainXHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);
    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UGainXHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void UGainXHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UGainXHealthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<AGainXGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;

    GameMode->Killed(KillerController, VictimController);
}

void UGainXHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Health <= 0.0f || IsDead() || !GetWorld()) return;
    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UGainXHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
    PlayCameraShake();
    ReportDamageEvent(Damage, InstigatedBy);
}

float UGainXHealthComponent::GetPointDamageModifaer(AActor* DamagedActor, const FName& BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character ||                                      //
        !Character->GetMesh() ||                           //
        !Character->GetMesh()->GetBodyInstance(BoneName))  //
        return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!DamageModifaers.Contains(PhysMaterial)) return 1.0f;
    return DamageModifaers[PhysMaterial];
}

void UGainXHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (!GetWorld() || !InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
    UAISense_Damage::ReportDamageEvent(GetWorld(),    //
        GetOwner(),                                   //
        InstigatedBy->GetPawn(),                      //
        Damage,                                       //
        InstigatedBy->GetPawn()->GetActorLocation(),  //
        GetOwner()->GetActorLocation());              //
}