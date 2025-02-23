// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GainXGameplayAbility.h"
#include "GainXGameplayAbility_Interact.generated.h"

/**
 *
 */
UCLASS()
class GAINX_API UGainXGameplayAbility_Interact : public UGainXGameplayAbility
{
    GENERATED_BODY()

public:
    UGainXGameplayAbility_Interact();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(EditDefaultsOnly)
    float InteractionScanRate = 0.1f;

    UPROPERTY(EditDefaultsOnly)
    float InteractionScanRange = 500.0f;
};
