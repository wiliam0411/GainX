// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DropSlotAbilityTask.generated.h"

class UGainXInventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropSlotAbilityTaskDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponDeployedEvent, UGainXInventoryItem*, Item);

/**
 *
 */
UCLASS()
class GAINX_API UDropSlotAbilityTask : public UAbilityTask
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "DropSlot", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UDropSlotAbilityTask* AbilityTask_DropSlot(UGameplayAbility* OwningAbility, UGainXQuickBarComponent* QuickBarComponent, UAnimInstance* AnimInstance, FGameplayTag DropMontageTag, FGameplayTag DropEventTag, FGameplayTag EquipMontageTag);

    virtual void Activate() override;
    virtual void OnDestroy(bool AbilityEnded) override;

    UPROPERTY(BlueprintAssignable)
    FOnWeaponDeployedEvent OnWeaponDeployedEvent;

    UPROPERTY(BlueprintAssignable)
    FOnDropSlotAbilityTaskDelegate OnDropMontageEndedEvent;

    UPROPERTY(BlueprintAssignable)
    FOnDropSlotAbilityTaskDelegate OnEquipMontageEndedEvent;

private:
    void PlayAnimMontage(FGameplayTag MontageTag, void (ThisClass::*OnMontageEndedFunc)());

    void OnWeaponDeployed(const FGameplayEventData* Payload);
    void OnDropMontageEnded();
    void OnEquipMontageEnded();
    
private:
    UPROPERTY()
    TObjectPtr<UGainXQuickBarComponent> QuickBar;

    UPROPERTY()
    TObjectPtr<UAnimInstance> AnimInstance;

    FGameplayTag DropMontageTag;
    FGameplayTag EquipMontageTag;
    FGameplayTag DropEventTag;

    FTimerHandle AbilityTaskTimerHandle;
};
