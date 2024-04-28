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
#include "AbilitySystem/GainXAbilitySet.h"
#include "Equipment/GainXEquipmentManagerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

AGainXBaseCharacter::AGainXBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<UGainXCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UGainXAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

    HealthSet = CreateDefaultSubobject<UGainXHealthSet>(TEXT("HealthSet"));

    NetUpdateFrequency = 100.0f;

    HealthComponent = CreateDefaultSubobject<UGainXHealthComponent>("HealthComponent");
    HealthComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
    LandedDelegate.AddDynamic(this, &AGainXBaseCharacter::OnGroundLanded);

    // WeaponComponent = CreateDefaultSubobject<UGainXWeaponComponent>("WeaponComponent");

    // TODO: Should be impl by gamefeatures
    EquipmentManagerComponent = CreateDefaultSubobject<UGainXEquipmentManagerComponent>("EquipmentManagerComponent");
}

AGainXPlayerState* AGainXBaseCharacter::GetGainXPlayerState() const
{
    if (!Controller || !Controller->PlayerState)
    {
        return nullptr;
    }

    return Cast<AGainXPlayerState>(Controller->PlayerState);
}

UGainXAbilitySystemComponent* AGainXBaseCharacter::GetGainXAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAbilitySystemComponent* AGainXBaseCharacter::GetAbilitySystemComponent() const
{
    return GetGainXAbilitySystemComponent();
}

void AGainXBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(AbilitySystemComponent);
    check(AbilitySet);
    check(HealthComponent);

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    AbilitySet->GiveToAbilitySystem(AbilitySystemComponent);

    HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
}

void AGainXBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Ќужно делать каждый тик чтобы обновл€ть прожатые абилки (лучше делать в контроллере)
    AbilitySystemComponent->ProcessAbilityInput();
}

void AGainXBaseCharacter::OnDeath()
{
    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);

    // Set collision to ignore mode
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

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