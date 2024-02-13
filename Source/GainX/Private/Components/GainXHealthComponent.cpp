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

    // Bind damage delegates
    if (AActor* ComponentOwner = GetOwner())
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
    const float FinalDamage = Damage * GetPointDamageModifaer(DamagedActor, BoneName);
    ApplyDamage(FinalDamage, InstigatedBy);
}

void UGainXHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
    const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
}

void UGainXHealthComponent::HealUpdate()
{
    // Calculate the amount of health the character should gain
    float HealthDelta = HealPerSecond * GetWorld()->GetTimerManager().GetTimerRate(HealTimerHandle);
    SetHealth(Health + HealthDelta);

    // Stop heal when health is full
    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UGainXHealthComponent::SetHealth(float NewHealth)
{
    const float ClampedNewHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const float HealthDelta = ClampedNewHealth - Health;
    Health = ClampedNewHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void UGainXHealthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<AGainXGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    GameMode->Killed(KillerController, Player->Controller);
}

void UGainXHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    // Clear heal timer if damaged when healing
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    // Character is dead
    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }

    // Character is alive, start auto heal
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(
            HealTimerHandle, this, &UGainXHealthComponent::HealUpdate, 1.0f / HealTimerUpdateRate, true, HealDelayTime);
    }

    PlayCameraShake();

    ReportDamageEvent(Damage, InstigatedBy);
}

float UGainXHealthComponent::GetPointDamageModifaer(AActor* DamagedActor, const FName& BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!DamageModifaers.Contains(PhysMaterial)) return 1.0f;

    return DamageModifaers[PhysMaterial];
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

void UGainXHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (!GetWorld() || !InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
    UAISense_Damage::ReportDamageEvent(               //
        GetWorld(),                                   //
        GetOwner(),                                   //
        InstigatedBy->GetPawn(),                      //
        Damage,                                       //
        InstigatedBy->GetPawn()->GetActorLocation(),  //
        GetOwner()->GetActorLocation());              //
}