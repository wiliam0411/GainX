// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask_LookForInteractables.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableTargetChangedEvent, FGameplayAbilitySpecHandle, InteractionAbilitySpecHandle);

/**
 *
 */
UCLASS()
class GAINX_API UAbilityTask_LookForInteractables : public UAbilityTask
{
    GENERATED_BODY()

public:
    UAbilityTask_LookForInteractables(const FObjectInitializer& ObjectInitializer);

    /* Wait until we trace new set of interactables.  This task automatically loops. */
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAbilityTask_LookForInteractables* LookForInteractables(UGameplayAbility* OwningAbility, ECollisionChannel InteractionCollisionChannel, float InteractionScanRange = 100.0f, float InteractionScanRate = 0.1f);

    virtual void Activate() override;
    virtual void OnDestroy(bool AbilityEnded) override;

public:
    UPROPERTY(BlueprintAssignable)
    FInteractableTargetChangedEvent InteractableObjectsChanged;

private:
    void PerformTrace();

protected:
    float ScanRate = 0.1f;
    float ScanRange = 100.0f;
    FTimerHandle ScanTimerHandle;
    ECollisionChannel ScanCollisionChannel;
};
