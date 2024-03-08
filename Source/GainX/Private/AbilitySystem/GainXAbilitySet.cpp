// GainX, All Rights Reserved

#include "AbilitySystem/GainXAbilitySet.h"
#include "AbilitySystem/GainXGameplayAbility.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

UGainXAbilitySet::UGainXAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXAbilitySet::GiveToAbilitySystem(UGainXAbilitySystemComponent* GainXASC) const
{
    check(GainXASC);

    // Must be authoritative to give or take ability sets
    if (!GainXASC->IsOwnerActorAuthoritative()) return;

    for (int32 AbilityIndex = 0; AbilityIndex < GameplayAbilities.Num(); ++AbilityIndex)
    {
        const FGainXAbilitySet_GameplayAbility& AbilityToGrant = GameplayAbilities[AbilityIndex];

        // Validation check
        if (!IsValid(AbilityToGrant.Ability)) continue;

        UGainXGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGainXGameplayAbility>();

        FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);

        AbilitySpec.SourceObject = nullptr;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

        const FGameplayAbilitySpecHandle AbilitySpecHandle = GainXASC->GiveAbility(AbilitySpec);
        UE_LOG(LogTemp, Display, TEXT("Ability has been given"));
    }
}
