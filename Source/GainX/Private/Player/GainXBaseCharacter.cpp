// GainX, All Rights Reserved

#include "Player/GainXBaseCharacter.h"
#include "Components/GainXCharacterMovementComponent.h"
#include "Components/GainXHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Components/GainXWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

AGainXBaseCharacter::AGainXBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UGainXCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UGainXHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<UGainXWeaponComponent>("WeaponComponent");
}

void AGainXBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &AGainXBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AGainXBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &AGainXBaseCharacter::OnGroundLanded);
}

void AGainXBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGainXBaseCharacter::TurnOff()
{
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    Super::TurnOff();
}

void AGainXBaseCharacter::Reset()
{
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    Super::Reset();
}

bool AGainXBaseCharacter::IsRunning() const
{
    return false;
}

float AGainXBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

void AGainXBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MatInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MatInst) return;

    MatInst->SetVectorParameterValue(MaterialColorName, Color);
}

void AGainXBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void AGainXBaseCharacter::OnHealthChanged(float Health, float HealthDelta) {}

void AGainXBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    if (FallVelocityZ < LandedDamageVelocity.X) return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}