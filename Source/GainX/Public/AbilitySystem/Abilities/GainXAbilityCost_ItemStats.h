// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GainXAbilityCost.h"
#include "GainXAbilityCost_ItemStats.generated.h"

/**
 * Represents a cost that requires expending a quantity of a tag stack on the associated item instance
 */
UCLASS(meta = (DisplayName = "Item Stats"))
class GAINX_API UGainXAbilityCost_ItemStats : public UGainXAbilityCost
{
    GENERATED_BODY()

public:
    UGainXAbilityCost_ItemStats();

    //~ULyraAbilityCost interface
    virtual bool CheckCost(                          //
        const UGainXGameplayAbility* Ability,        //
        const FGameplayAbilitySpecHandle Handle,     //
        const FGameplayAbilityActorInfo* ActorInfo,  //
        FGameplayTagContainer* OptionalRelevantTags) const override;

    virtual void ApplyCost(                          //
        const UGainXGameplayAbility* Ability,        //
        const FGameplayAbilitySpecHandle Handle,     //
        const FGameplayAbilityActorInfo* ActorInfo,  //
        const FGameplayAbilityActivationInfo ActivationInfo) override;
    //~End of ULyraAbilityCost interface

protected:
    /** How much of the tag to spend (keyed on ability level) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
    int32 Quantity = 1;

    /** Which tag to spend some of */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
    FGameplayTag Tag;

    /** Which tag to send back as a response if this cost cannot be applied */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
    FGameplayTag FailureTag;
};
