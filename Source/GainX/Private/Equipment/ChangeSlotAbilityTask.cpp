// GainX, All Rights Reserved

#include "Equipment/ChangeSlotAbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Player/GainXBaseCharacter.h"
#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentActor.h"

UChangeSlotAbilityTask* UChangeSlotAbilityTask::ChangeSlot(UGameplayAbility* OwningAbility, int32 NewIndex, UGainXQuickBarComponent* QuickBarComponent, UAnimInstance* AnimInstance, FGameplayTag EquipMontageTag, FGameplayTag UnequipMontageTag)
{
    UChangeSlotAbilityTask* MyObj = NewAbilityTask<UChangeSlotAbilityTask>(OwningAbility, NAME_None);
    MyObj->QuickBar = QuickBarComponent;
    MyObj->EquipMontageTag = EquipMontageTag;
    MyObj->UnequipMontageTag = UnequipMontageTag;
    MyObj->NewIndex = NewIndex;
    MyObj->AnimInstance = AnimInstance;

    return MyObj;
}

void UChangeSlotAbilityTask::Activate()
{
    PlayAnimMontage(UnequipMontageTag, &UChangeSlotAbilityTask::OnUnequipMontageEnded);
}

void UChangeSlotAbilityTask::OnUnequipMontageEnded()
{
    OnUnequipMontageEndedEvent.Broadcast();

    check(QuickBar);

    QuickBar->UnequipItemInSlot();

    QuickBar->ChangeActiveSlotIndex(NewIndex);

    QuickBar->EquipItemInSlot();

    PlayAnimMontage(EquipMontageTag, &UChangeSlotAbilityTask::OnEquipMontageEnded);
}

void UChangeSlotAbilityTask::OnEquipMontageEnded()
{
    OnEquipMontageEndedEvent.Broadcast();
}

void UChangeSlotAbilityTask::PlayAnimMontage(FGameplayTag MontageTag, void (ThisClass::*OnMontageEndedFunc)())
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
