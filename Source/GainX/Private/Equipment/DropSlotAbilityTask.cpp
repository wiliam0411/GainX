// GainX, All Rights Reserved

#include "Equipment/DropSlotAbilityTask.h"
#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentActor.h"
#include "AbilitySystemComponent.h"

UDropSlotAbilityTask* UDropSlotAbilityTask::AbilityTask_DropSlot(UGameplayAbility* OwningAbility, UGainXQuickBarComponent* QuickBarComponent, UAnimInstance* AnimInstance, FGameplayTag DropMontageTag, FGameplayTag DropEventTag, FGameplayTag EquipMontageTag)
{
    UDropSlotAbilityTask* MyObj = NewAbilityTask<UDropSlotAbilityTask>(OwningAbility, NAME_None);
    MyObj->QuickBar = QuickBarComponent;
    MyObj->AnimInstance = AnimInstance;
    MyObj->DropMontageTag = DropMontageTag;
    MyObj->DropEventTag = DropEventTag;
    MyObj->EquipMontageTag = EquipMontageTag;

    return MyObj;
}

void UDropSlotAbilityTask::Activate()
{
    PlayAnimMontage(DropMontageTag, &UDropSlotAbilityTask::OnDropMontageEnded);

    UAbilitySystemComponent* ASC = Ability->GetAbilitySystemComponentFromActorInfo();
    if (ensure(ASC))
    {
        ASC->GenericGameplayEventCallbacks.FindOrAdd(DropEventTag).AddUObject(this, &UDropSlotAbilityTask::OnWeaponDeployed);
    }
}

void UDropSlotAbilityTask::OnDestroy(bool AbilityEnded) 
{
    AbilityTaskTimerHandle.Invalidate();

    Super::OnDestroy(AbilityEnded);
}

void UDropSlotAbilityTask::PlayAnimMontage(FGameplayTag MontageTag, void (ThisClass::*OnMontageEndedFunc)())
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

void UDropSlotAbilityTask::OnDropMontageEnded()
{
    QuickBar->ChangeActiveSlotIndex(QuickBar->GetFirstFilledSlotIndex());

    QuickBar->EquipItemInSlot();

    PlayAnimMontage(EquipMontageTag, &UDropSlotAbilityTask::OnEquipMontageEnded);

    OnDropMontageEndedEvent.Broadcast();
}

void UDropSlotAbilityTask::OnEquipMontageEnded()
{
    OnEquipMontageEndedEvent.Broadcast();
}

void UDropSlotAbilityTask::OnWeaponDeployed(const FGameplayEventData* Payload)
{
    UGainXInventoryItem* RemovedItem = QuickBar->RemoveItemFromSlot(QuickBar->GetActiveSlotIndex());
    OnWeaponDeployedEvent.Broadcast(RemovedItem);

    QuickBar->UnequipItemInSlot();
}