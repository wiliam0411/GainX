// GainX, All Rights Reserved

#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Equipment/GainXEquipmentActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXQuickBarComponent, All, All)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Message_SlotsChanged, "GainX.QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_QuickBar_Message_ActiveIndexChanged, "GainX.QuickBar.Message.ActiveIndexChanged");

UGainXQuickBarComponent::UGainXQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXQuickBarComponent::CycleActiveSlotForward()
{
    if (Slots.Num() < 2)
    {
        return;
    }

    const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
    int32 NewIndex = ActiveSlotIndex;
    do
    {
        NewIndex = (NewIndex + 1) % Slots.Num();
        if (Slots[NewIndex] != nullptr)
        {
            SetActiveSlotIndex(NewIndex);
            return;
        }
    } while (NewIndex != OldIndex);
}

void UGainXQuickBarComponent::CycleActiveSlotBackward()
{
    if (Slots.Num() < 2)
    {
        return;
    }

    const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
    int32 NewIndex = ActiveSlotIndex;
    do
    {
        NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
        if (Slots[NewIndex] != nullptr)
        {
            SetActiveSlotIndex(NewIndex);
            return;
        }
    } while (NewIndex != OldIndex);
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

bool UGainXQuickBarComponent::AddItemToFirstFreeSlot(UGainXInventoryItem* InventoryItem)
{
    for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
    {
        if (AddItemToSlot(SlotIndex, InventoryItem))
        {
            // If there was no slot active, set the first filled slot as active
            if (ActiveSlotIndex == -1)
            {
                SetActiveSlotIndex(SlotIndex);
            }

            return true;
        }
    }

    return false;
}

UGainXInventoryItem* UGainXQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
    // Case when we remove active equip item, need to destroy all spawned actors by EquipmentManager
    if (ActiveSlotIndex == SlotIndex)
    {
        UnequipItemInSlot();
        ActiveSlotIndex = -1;
    }

    // If slot to be deleted is not active, simply set it to nullptr
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

UGainXInventoryItem* UGainXQuickBarComponent::RemoveItemFromActiveSlot()
{
    if (Slots.IsValidIndex(ActiveSlotIndex) && Slots[ActiveSlotIndex] != nullptr)
    {
        // Cache pointer on removed item in order to return it
        UGainXInventoryItem* RemovedItem = Slots[ActiveSlotIndex];

        // Clear slot
        Slots[ActiveSlotIndex] = nullptr;

        // Broadcast message to UI
        FGainXQuickBarSlotsChangedMessage Message;
        Message.Owner = GetOwner();
        Message.Slots = Slots;

        UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
        MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_SlotsChanged, Message);



        // Trying to find first filled slot and set it as active
        for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
        {
            if (Slots[SlotIndex] != nullptr)
            {
                SetActiveSlotIndex(SlotIndex);

                return RemovedItem;
            }
        }

        // If no filled slots found, switching to hands
        SetActiveSlotIndex(-1);

        return RemovedItem;
    }

    return nullptr;
}

void UGainXQuickBarComponent::BeginPlay()
{
    if (Slots.Num() < NumSlots)
    {
        Slots.AddDefaulted(NumSlots - Slots.Num());
    }

    Super::BeginPlay();
}

void UGainXQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
    if (ActiveSlotIndex == NewIndex || (!Slots.IsValidIndex(NewIndex) && NewIndex != -1))
    {
        return;
    }

    UnequipItemInSlot();

    ActiveSlotIndex = NewIndex;

    if (NewIndex != -1)
    {
        EquipItemInSlot();
    }

    // Creating and broadcasting message for UI
    FGainXQuickBarActiveIndexChangedMessage Message;
    Message.Owner = GetOwner();
    Message.ActiveIndex = ActiveSlotIndex;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
    MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_ActiveIndexChanged, Message);
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
    check(Slots.IsValidIndex(ActiveSlotIndex));
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
        // Check if item is not empty
        if (EquippedItem != nullptr)
        {
            EquipmentManager->UnequipItem(EquippedItem);
            EquippedItem = nullptr;
        }
    }
}
