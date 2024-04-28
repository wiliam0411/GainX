// GainX, All Rights Reserved

#include "AbilitySystem/GainXAbilitySet.h"
#include "AbilitySystem/GainXGameplayAbility.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

UGainXAbilitySet::UGainXAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXAbilitySet::GiveToAbilitySystem(UGainXAbilitySystemComponent* GainXASC, UObject* SourceObject) const
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
    }
}
