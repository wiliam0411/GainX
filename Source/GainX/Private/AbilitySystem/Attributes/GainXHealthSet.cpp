// GainX, All Rights Reserved

#include "AbilitySystem/Attributes/GainXHealthSet.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

UGainXHealthSet::UGainXHealthSet() : Health(100.0f), MaxHealth(100.0f)
{
    bOutOfHealth = false;
    HealthBeforeAttributeChange = 0.0f;
    MaxHealthBeforeAttributeChange = 0.0f;
}

bool UGainXHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    if (!Super::PreGameplayEffectExecute(Data))
    {
        return false;
    }

    // Save the current health
    HealthBeforeAttributeChange = GetHealth();
    MaxHealthBeforeAttributeChange = GetMaxHealth();

    return true;
}

void UGainXHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    float MinimumHealth = 0.0f;

    const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
    AActor* Instigator = EffectContext.GetOriginalInstigator();
    AActor* Causer = EffectContext.GetEffectCauser();

    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        // Convert into -Health and then clamp
        SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
        SetDamage(0.0f);
    }

    else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
    {
        // Convert into +Health and then clamp
        SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
        SetHealing(0.0f);
    }

    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Clamp and fall into out of health handling below
        SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
    }

    else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
    {
        OnMaxHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxHealthBeforeAttributeChange, GetMaxHealth());
    }

    if (GetHealth() != HealthBeforeAttributeChange)
    {
        OnHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
    }

    if ((GetHealth() <= 0.0f) && !bOutOfHealth)
    {
        OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
    }

    // Check health again in case an event above changed it.
    bOutOfHealth = (GetHealth() <= 0.0f);
}

void UGainXHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    Super::PreAttributeBaseChange(Attribute, NewValue);

    ClampAttribute(Attribute, NewValue);
}

void UGainXHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    ClampAttribute(Attribute, NewValue);
}

void UGainXHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    // Make sure current Health is not greater than the new MaxHealth
    if (Attribute == GetHealthAttribute() && GetHealth() > NewValue)
    {
        UGainXAbilitySystemComponent* GainXASC = GetGainXAbilitySystemComponent();
        check(GainXASC);

        GainXASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
    }

    if (bOutOfHealth && (GetHealth() > 0.0f))
    {
        bOutOfHealth = false;
    }
}

void UGainXHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    else if (Attribute == GetMaxHealthAttribute())
    {
        NewValue = FMath::Max(NewValue, 1.0f);
    }
}
