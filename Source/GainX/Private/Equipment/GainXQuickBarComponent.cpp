// GainX, All Rights Reserved

#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Equipment/GainXEquipmentActor.h"

#include "Player/GainXBaseCharacter.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXQuickBarComponent, All, All)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Message_SlotsChanged, "GainX.QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Message_ActiveIndexChanged, "GainX.QuickBar.Message.ActiveIndexChanged");

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_UnequipTest, "GainX.QuickBar.UnequipTest");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Animation_Unequip, "GainX.QuickBar.Animation.Unequip");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Animation_Equip, "GainX.QuickBar.Animation.Equip");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Animation_Drop, "GainX.QuickBar.Animation.Drop");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_Ability_Block_QuickBar, "Ability.Block.QuickBar");

UGainXQuickBarComponent::UGainXQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    Slots.AddDefaulted(3);
}

bool UGainXQuickBarComponent::AddItemToSlot(int32 SlotIndex, UGainXInventoryItem* InventoryItem)
{
    // Check if the slot index is valid and the inventory item is not null
    if (!Slots.IsValidIndex(SlotIndex) || !InventoryItem)
    {
        return false;
    }

    // Add item only if slot is empty
    if (Slots[SlotIndex] == nullptr)
    {
        Slots[SlotIndex] = InventoryItem;

        FGainXQuickBarSlotsChangedMessage Message;
        Message.Owner = GetOwner();
        Message.Slots = Slots;

        UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
        MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_SlotsChanged, Message);

        return true;
    }

    // Case when slot is already filled
    return false;
}

UGainXInventoryItem* UGainXQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
    if (Slots.IsValidIndex(SlotIndex))
    {
        if (Slots[SlotIndex] != nullptr)
        {
            // Cache pointer on removed item in order to return it
            UGainXInventoryItem* RemovedItem = Slots[SlotIndex];

            // Clear slot
            Slots[SlotIndex] = nullptr;

            // Broadcast message to UI
            FGainXQuickBarSlotsChangedMessage Message;
            Message.Owner = GetOwner();
            Message.Slots = Slots;

            UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
            MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_SlotsChanged, Message);

            return RemovedItem;
        }
    }

    return nullptr;
}

void UGainXQuickBarComponent::ChangeActiveSlotIndex(int32 NewIndex)
{
    // If index is the same or out of range, we are using -1 for arms
    if (ActiveSlotIndex == NewIndex || (!Slots.IsValidIndex(NewIndex) && NewIndex != -1))
    {
        return;
    }

    ActiveSlotIndex = NewIndex;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
    FGainXQuickBarActiveIndexChangedMessage Message;
    Message.Owner = GetOwner();
    Message.ActiveIndex = ActiveSlotIndex;
    MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_ActiveIndexChanged, Message);
}

int32 UGainXQuickBarComponent::GetNextSlotIndex() const
{
    const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
    int32 NewIndex = ActiveSlotIndex;
    do
    {
        NewIndex = (NewIndex + 1) % Slots.Num();
        if (Slots[NewIndex] != nullptr)
        {
            return NewIndex;
        }
    } while (NewIndex != OldIndex);

    return -1;
}

int32 UGainXQuickBarComponent::GetPreviousSlotIndex() const
{
    const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
    int32 NewIndex = ActiveSlotIndex;
    do
    {
        NewIndex = (NewIndex - 1) % Slots.Num();
        if (Slots[NewIndex] != nullptr)
        {
            return NewIndex;
        }
    } while (NewIndex != OldIndex);

    return -1;
}

int32 UGainXQuickBarComponent::GetFirstFilledSlotIndex() const
{
    for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
    {
        if (Slots[SlotIndex] != nullptr)
        {
            return SlotIndex;
        }
    }

    return -1;
}

int32 UGainXQuickBarComponent::GetFirstFreeSlotIndex() const
{
    for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
    {
        if (Slots[SlotIndex] == nullptr)
        {
            return SlotIndex;
        }
    }
    return -1;
}

UGainXEquipmentManagerComponent* UGainXQuickBarComponent::FindEquipmentManagerComponent() const
{
    if (AController* OwnerController = Cast<AController>(GetOwner()))
    {
        if (APawn* OwnerPawn = OwnerController->GetPawn())
        {
            return OwnerPawn->FindComponentByClass<UGainXEquipmentManagerComponent>();
        }
    }

    return nullptr;
}

void UGainXQuickBarComponent::EquipItemInSlot()
{
    if (!Slots.IsValidIndex(ActiveSlotIndex))
    {
        return;
    }

    check(EquippedItem == nullptr);

    if (UGainXInventoryItem* SlotItem = Slots[ActiveSlotIndex])
    {
        if (const auto EquippableItemFragment = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
        {
            if (TSubclassOf<AGainXEquipmentActor> EquipmentItemClass = EquippableItemFragment->EquipmentObject)
            {
                if (UGainXEquipmentManagerComponent* EquipmentManager = FindEquipmentManagerComponent())
                {
                    EquippedItem = EquipmentManager->EquipItem(EquipmentItemClass, SlotItem);
                    return;
                }
            }
        }
    }
}

void UGainXQuickBarComponent::UnequipItemInSlot()
{
    if (UGainXEquipmentManagerComponent* EquipmentManager = FindEquipmentManagerComponent())
    {
        if (EquippedItem != nullptr)
        {
            EquipmentManager->UnequipItem(EquippedItem);

            EquippedItem = nullptr;
        }
    }
}