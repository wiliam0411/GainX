// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbilityCost.h"

UGainXAbilityCost::UGainXAbilityCost() {}

bool UGainXAbilityCost::CheckCost(               //
    const UGainXGameplayAbility* Ability,        //
    const FGameplayAbilitySpecHandle Handle,     //
    const FGameplayAbilityActorInfo* ActorInfo,  //
    FGameplayTagContainer* OptionalRelevantTags) const
{
    return true;
}

void UGainXAbilityCost::ApplyCost(               //
    const UGainXGameplayAbility* Ability,        //
    const FGameplayAbilitySpecHandle Handle,     //
    const FGameplayAbilityActorInfo* ActorInfo,  //
    const FGameplayAbilityActivationInfo ActivationInfo)
{
}