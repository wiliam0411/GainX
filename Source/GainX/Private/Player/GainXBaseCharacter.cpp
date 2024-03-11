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

#include "Player/GainXPlayerState.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

AGainXBaseCharacter::AGainXBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<UGainXCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UGainXHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<UGainXWeaponComponent>("WeaponComponent");
}

AGainXPlayerState* AGainXBaseCharacter::GetGainXPlayerState()
{
    if (!Controller) return nullptr;
    return Cast<AGainXPlayerState>(Controller->PlayerState);
}

UGainXAbilitySystemComponent* AGainXBaseCharacter::GetGainXAbilitySystemComponent() const
{
    auto GainXPlayerState = Cast<AGainXPlayerState>(Controller->PlayerState);
    if (!GainXPlayerState) return nullptr;
    return GainXPlayerState->GetGainXAbilitySystemComponent();
}

UAbilitySystemComponent* AGainXBaseCharacter::GetAbilitySystemComponent() const
{
    return GetGainXAbilitySystemComponent();
}

void AGainXBaseCharacter::InitializeAbilitySystem(UGainXAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
    check(InASC);
    check(InOwnerActor);

    // The ability system component hasn't changed.
    if (AbilitySystemComponent == InASC) return;

    // Clean up the old ability system component if it exists
    UninitializeAbilitySystem();

    AbilitySystemComponent = InASC;
    AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, this);
}

void AGainXBaseCharacter::UninitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
    {

        AbilitySystemComponent->ClearAbilityInput();
        AbilitySystemComponent->RemoveAllGameplayCues();

        if (!AbilitySystemComponent->GetOwnerActor())
        {
            AbilitySystemComponent->SetAvatarActor(nullptr);
        }
        else
        {
            // If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
            AbilitySystemComponent->ClearActorInfo();
        }
    }

    AbilitySystemComponent = nullptr;
}

void AGainXBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);

    // Death and HealthChanged delegates bind
    HealthComponent->OnDeath.AddUObject(this, &AGainXBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AGainXBaseCharacter::OnHealthChanged);

    // Landing delegate bind
    LandedDelegate.AddDynamic(this, &AGainXBaseCharacter::OnGroundLanded);
}

void AGainXBaseCharacter::OnDeath()
{
    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);

    // Set collision to ignore mode
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);

    // Animation of death
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    // Play the sound of death
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound.Get(), GetActorLocation());
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

void AGainXBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    if (const auto MatInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
    {
        MatInst->SetVectorParameterValue(MaterialColorParameterName, Color);
    }
}

void AGainXBaseCharacter::OnHealthChanged(float Health, float HealthDelta) {}

void AGainXBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const float FallVelocity = -GetVelocity().Z;
    if (FallVelocity > FallDamageVelocityThreshold)
    {
        const float FallDamage = FMath::Clamp(FallDamageMultiplier * (FallVelocity - FallDamageVelocityThreshold), 0.0f, HealthComponent->GetHealth());
        UE_LOG(LogBaseCharacter, Display, TEXT("Fall damage: %f"), FallDamage);
        TakeDamage(FallDamage, FPointDamageEvent{}, nullptr, nullptr);
    }
}