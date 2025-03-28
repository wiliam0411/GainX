// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ChangeSlotAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeSlotAbilityTaskDelegate);

class UGainXQuickBarComponent;

/**
 *
 */
UCLASS()
class GAINX_API UChangeSlotAbilityTask : public UAbilityTask
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FChangeSlotAbilityTaskDelegate OnUnequipMontageEndedEvent;

    UPROPERTY(BlueprintAssignable)
    FChangeSlotAbilityTaskDelegate OnEquipMontageEndedEvent;

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ChangeSlot", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UChangeSlotAbilityTask* ChangeSlot(UGameplayAbility* OwningAbility, int32 NewIndex, UGainXQuickBarComponent* QuickBarComponent, UAnimInstance* AnimInstance, FGameplayTag EquipMontageTag, FGameplayTag UnequipMontageTag);

    virtual void Activate() override;

protected:
    UFUNCTION()
    void OnUnequipMontageEnded();

    UFUNCTION()
    void OnEquipMontageEnded();

    void PlayAnimMontage(FGameplayTag MontageTag, void (ThisClass::*OnMontageEndedFunc)());

private:
    int32 NewIndex;

    TObjectPtr<UAnimInstance> AnimInstance;

    FGameplayTag EquipMontageTag;

    FGameplayTag UnequipMontageTag;

    TObjectPtr<UGainXQuickBarComponent> QuickBar;

    FTimerHandle AbilityTaskTimerHandle;
};
