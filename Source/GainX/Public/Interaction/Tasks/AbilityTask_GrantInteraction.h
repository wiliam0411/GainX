// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Interaction/InteractableTarget.h"

#include "AbilityTask_GrantInteraction.generated.h"

/**
 *
 */
UCLASS()
class GAINX_API UAbilityTask_GrantInteraction : public UAbilityTask
{
    GENERATED_BODY()

public:
    UAbilityTask_GrantInteraction(const FObjectInitializer& ObjectInitializer);

    virtual void Activate() override;
    virtual void OnDestroy(bool AbilityEnded) override;

    /** Wait until an overlap occurs. This will need to be better fleshed out so we can specify game specific collision requirements */
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAbilityTask_GrantInteraction* GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate);

private:
    void QueryInteractables();

private:
    float InteractionScanRange = 100.0f;
    float InteractionScanRate = 0.1f;
    FTimerHandle QueryTimerHandle;
    TMap<TScriptInterface<IInteractableTarget>, FGameplayAbilitySpecHandle> InteractableTargets;
};
