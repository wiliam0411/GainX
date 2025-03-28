// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AddSlotAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddSlotAbilityTaskDelegate);

/**
 *
 */

class UGainXInventoryItem;

UCLASS()
class GAINX_API UAddSlotAbilityTask : public UAbilityTask
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "AddSlot", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAddSlotAbilityTask* AbilityTask_AddSlot(UGameplayAbility* OwningAbility, UGainXQuickBarComponent* QuickBarComponent, UAnimInstance* AnimInstance, UGainXInventoryItem* InventoryItem, FGameplayTag UnequipMontageTag, FGameplayTag EquipMontageTag);

    UPROPERTY(BlueprintAssignable)
    FOnAddSlotAbilityTaskDelegate OnEquipMontageEndedEvent;

    virtual void Activate() override;

private:
    void PlayAnimMontage(FGameplayTag MontageTag, void (ThisClass::*OnMontageEndedFunc)());
    void OnUnequipMontageEnded();
    void OnEquipMontageEnded();

private:
    UPROPERTY()
    TObjectPtr<UGainXQuickBarComponent> QuickBar;

    UPROPERTY()
    TObjectPtr<UAnimInstance> AnimInstance;

    UPROPERTY()
    TObjectPtr<UGainXInventoryItem> InventoryItem;

    FGameplayTag UnequipMontageTag;
    FGameplayTag EquipMontageTag;

    FTimerHandle AbilityTaskTimerHandle;
};
