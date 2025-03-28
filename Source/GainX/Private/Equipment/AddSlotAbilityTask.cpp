// GainX, All Rights Reserved

#include "Equipment/AddSlotAbilityTask.h"
#include "Equipment/GainXEquipmentActor.h"
#include "Equipment/GainXQuickBarComponent.h"

UAddSlotAbilityTask* UAddSlotAbilityTask::AbilityTask_AddSlot(UGameplayAbility* OwningAbility, UGainXQuickBarComponent* QuickBarComponent, UAnimInstance* AnimInstance, UGainXInventoryItem* InventoryItem, FGameplayTag UnequipMontageTag, FGameplayTag EquipMontageTag)
{
    UAddSlotAbilityTask* MyObj = NewAbilityTask<UAddSlotAbilityTask>(OwningAbility, NAME_None);
    MyObj->QuickBar = QuickBarComponent;
    MyObj->AnimInstance = AnimInstance;
    MyObj->InventoryItem = InventoryItem;
    MyObj->UnequipMontageTag = UnequipMontageTag;
    MyObj->EquipMontageTag = EquipMontageTag;

    return MyObj;
}

void UAddSlotAbilityTask::Activate()
{
    int32 FreeSlotIndex = QuickBar->GetFirstFreeSlotIndex();

    if (QuickBar->AddItemToSlot(FreeSlotIndex, InventoryItem))
    {
        if (QuickBar->GetActiveSlotIndex() == -1)
        {
            QuickBar->ChangeActiveSlotIndex(FreeSlotIndex);

            QuickBar->EquipItemInSlot();

            PlayAnimMontage(EquipMontageTag, &UAddSlotAbilityTask::OnEquipMontageEnded);
        }
    }
    else
    {
        OnEquipMontageEnded();
    }
}

void UAddSlotAbilityTask::PlayAnimMontage(FGameplayTag MontageTag, void (ThisClass::*OnMontageEndedFunc)())
{
    if (ensure(AnimInstance))
    {
        if (ensure(QuickBar))
        {
            if (AGainXEquipmentActor* EquipmentActor = QuickBar->GetEquipmentActor())
            {
                if (UAnimMontage* Montage = EquipmentActor->GetAnimMontageByTag(MontageTag))
                {
                    AnimInstance->Montage_Play(Montage);
                    GetWorld()->GetTimerManager().SetTimer(AbilityTaskTimerHandle, this, OnMontageEndedFunc, Montage->GetPlayLength(), false);

                    return;
                }
            }
        }
    }

    (this->*OnMontageEndedFunc)();
}

void UAddSlotAbilityTask::OnUnequipMontageEnded() 
{

}

void UAddSlotAbilityTask::OnEquipMontageEnded() 
{
    OnEquipMontageEndedEvent.Broadcast();
}
