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
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GainXHealthSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GainXBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UGainXHealthComponent::UGainXHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = false;

    AbilitySystemComponent = nullptr;
    HealthSet = nullptr;
}

UGainXHealthComponent* UGainXHealthComponent::FindHealthComponent(const AActor* Actor)
{
    return Actor ? Actor->FindComponentByClass<UGainXHealthComponent>() : nullptr;
}

void UGainXHealthComponent::InitializeWithAbilitySystem(UGainXAbilitySystemComponent* InASC)
{
    AActor* Owner = GetOwner();
    check(Owner);

    // Case when Ability System Component is alredy initialized
    if (AbilitySystemComponent)
    {
        UE_LOG(LogHealthComponent, Error, TEXT("GainXHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
        return;
    }

    // Update local duplicate Ability System Component
    AbilitySystemComponent = InASC;
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogHealthComponent, Error, TEXT("GainXHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
        return;
    }

    // Update local duplicate HealthSet
    HealthSet = AbilitySystemComponent->GetSet<UGainXHealthSet>();
    if (!HealthSet)
    {
        UE_LOG(LogHealthComponent, Error, TEXT("GainXHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
        return;
    }

    // Register to listen for attribute changes
    HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
    HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
    HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

    // Reset attributes to default values
    AbilitySystemComponent->SetNumericAttributeBase(UGainXHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());
}

void UGainXHealthComponent::UninitializeFromAbilitySystem()
{
    // Clear all delegates
    if (HealthSet)
    {
        HealthSet->OnHealthChanged.RemoveAll(this);
        HealthSet->OnMaxHealthChanged.RemoveAll(this);
        HealthSet->OnOutOfHealth.RemoveAll(this);
    }

    HealthSet = nullptr;
    AbilitySystemComponent = nullptr;
}

float UGainXHealthComponent::GetHealth() const
{
    if (!HealthSet)
    {
        return 0.0f;
    }

    return HealthSet->GetHealth();
}

float UGainXHealthComponent::GetMaxHealth() const
{
    if (!HealthSet)
    {
        return 0.0f;
    }

    return HealthSet->GetMaxHealth();
}

float UGainXHealthComponent::GetHealthNormalized() const
{
    if (!HealthSet)
    {
        return 0.0f;
    }

    return UKismetMathLibrary::SafeDivide(HealthSet->GetHealth(), HealthSet->GetMaxHealth());
}

bool UGainXHealthComponent::IsDead() const
{
    if (!HealthSet)
    {
        return true;
    }
    return FMath::IsNearlyZero(HealthSet->GetHealth());
}

bool UGainXHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(HealthSet->GetHealth(), HealthSet->GetMaxHealth());
}

void UGainXHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
    OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UGainXHealthComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
    OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UGainXHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
    OnDeath.Broadcast(GetOwner());
}

void UGainXHealthComponent::OnUnregister()
{
    UninitializeFromAbilitySystem();

    Super::OnUnregister();
}