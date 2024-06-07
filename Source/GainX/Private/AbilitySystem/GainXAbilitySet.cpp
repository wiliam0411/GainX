// GainX, All Rights Reserved

#include "AbilitySystem/GainXAbilitySet.h"
#include "AbilitySystem/GainXGameplayAbility.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

UGainXAbilitySet::UGainXAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXAbilitySet::GiveToAbilitySystem(UGainXAbilitySystemComponent* GainXASC, FGainXAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
    check(GainXASC);

    // Must be authoritative to give or take ability sets
    if (!GainXASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    // Grant the gameplay abilities
    for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
    {
        const FGainXAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

        if (!IsValid(AbilityToGrant.Ability))
        {
            continue;
        }

        UGainXGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGainXGameplayAbility>();

        FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
        AbilitySpec.SourceObject = SourceObject;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

        const FGameplayAbilitySpecHandle AbilitySpecHandle = GainXASC->GiveAbility(AbilitySpec);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
        }
    }

    // Grant the attribute sets
    for (int32 AttributeSetIndex = 0; AttributeSetIndex < GrantedGameplayAttributeSets.Num(); ++AttributeSetIndex)
    {
        const FGainXAbilitySet_AttributeSet& SetToGrant = GrantedGameplayAttributeSets[AttributeSetIndex];

        if (!IsValid(SetToGrant.AttributeSet))
        {
            continue;
        }

        UAttributeSet* NewSet = NewObject<UAttributeSet>(GainXASC->GetOwner(), SetToGrant.AttributeSet);
        GainXASC->AddAttributeSetSubobject(NewSet);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAttributeSet(NewSet);
        }
    }

    // Grant the gameplay effects.
    for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
    {
        const FGainXAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

        if (!IsValid(EffectToGrant.GameplayEffect))
        {
            continue;
        }

        const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
        const FActiveGameplayEffectHandle GameplayEffectHandle = GainXASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, GainXASC->MakeEffectContext());

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
        }
    }
}

void FGainXAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
    if (Handle.IsValid())
    {
        AbilitySpecHandles.Add(Handle);
    }
}

void FGainXAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
    if (Handle.IsValid())
    {
        GameplayEffectHandles.Add(Handle);
    }
}

void FGainXAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
    GrantedAttributeSets.Add(Set);
}

void FGainXAbilitySet_GrantedHandles::TakeFromAbilitySystem(UGainXAbilitySystemComponent* GainXASC)
{
    check(GainXASC);

    // Must be authoritative to give or take ability sets.
    if (!GainXASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
    {
        if (Handle.IsValid())
        {
            GainXASC->ClearAbility(Handle);
        }
    }

    for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
    {
        if (Handle.IsValid())
        {
            GainXASC->RemoveActiveGameplayEffect(Handle);
        }
    }

    for (UAttributeSet* Set : GrantedAttributeSets)
    {
        GainXASC->RemoveSpawnedAttribute(Set);
    }

    AbilitySpecHandles.Reset();
    GameplayEffectHandles.Reset();
    GrantedAttributeSets.Reset();
}