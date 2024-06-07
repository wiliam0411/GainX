// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "GainXAbilityCost.generated.h"

class UGainXGameplayAbility;

/**
 * Base class for costs that a UGainXGameplayAbility has (e.g., ammo or charges)
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class GAINX_API UGainXAbilityCost : public UObject
{
    GENERATED_BODY()

public:
    UGainXAbilityCost();

    /**
     * Checks if we can afford this cost.
     *
     * A failure reason tag can be added to OptionalRelevantTags (if non-null), which can be queried
     * elsewhere to determine how to provide user feedback (e.g., a clicking noise if a weapon is out of ammo)
     *
     * Ability and ActorInfo are guaranteed to be non-null on entry, but OptionalRelevantTags can be nullptr.
     *
     * @return true if we can pay for the ability, false otherwise.
     */
    virtual bool CheckCost(                          //
        const UGainXGameplayAbility* Ability,        //
        const FGameplayAbilitySpecHandle Handle,     //
        const FGameplayAbilityActorInfo* ActorInfo,  //
        FGameplayTagContainer* OptionalRelevantTags) const;

    /**
     * Applies the ability's cost to the target
     */
    virtual void ApplyCost(                          //
        const UGainXGameplayAbility* Ability,        //
        const FGameplayAbilitySpecHandle Handle,     //
        const FGameplayAbilityActorInfo* ActorInfo,  //
        const FGameplayAbilityActivationInfo ActivationInfo);
};