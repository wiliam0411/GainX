// GainX, All Rights Reserved

#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Equipment/GainXEquipmentObject.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"

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

void UGainXQuickBarComponent::AddItemToSlot(int32 SlotIndex, UGainXInventoryItem* InventoryItem)
{
    if (Slots.IsValidIndex(SlotIndex) && InventoryItem)
    {
        if (Slots[SlotIndex] == nullptr)
        {
            Slots[SlotIndex] = InventoryItem;

            FGainXQuickBarSlotsChangedMessage Message;
            Message.Owner = GetOwner();
            Message.Slots = Slots;

            UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
            MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_SlotsChanged, Message);
        }
    }
}

UGainXInventoryItem* UGainXQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
    UGainXInventoryItem* Result = nullptr;

    if (ActiveSlotIndex == SlotIndex)
    {
        UnequipItemInSlot();
        ActiveSlotIndex = -1;
    }

    if (Slots.IsValidIndex(SlotIndex))
    {
        Result = Slots[SlotIndex];

        if (Result != nullptr)
        {
            Slots[SlotIndex] = nullptr;

            FGainXQuickBarSlotsChangedMessage Message;
            Message.Owner = GetOwner();
            Message.Slots = Slots;

            UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
            MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_SlotsChanged, Message);
        }
    }

    return Result;
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
    if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
    {
        UnequipItemInSlot();

        ActiveSlotIndex = NewIndex;

        EquipItemInSlot();

        FGainXQuickBarActiveIndexChangedMessage Message;
        Message.Owner = GetOwner();
        Message.ActiveIndex = ActiveSlotIndex;

        UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
        MessageSystem.BroadcastMessage(TAG_GainX_QuickBar_Message_ActiveIndexChanged, Message);
    }
}

UGainXEquipmentManagerComponent* UGainXQuickBarComponent::FindEquipmentManagerComponent() const
{
    AController* OwnerController = Cast<AController>(GetOwner());
    if (!OwnerController)
    {
        return nullptr;
    }

    APawn* OwnerPawn = OwnerController->GetPawn();
    if (!OwnerPawn)
    {
        return nullptr;
    }

    return OwnerPawn->FindComponentByClass<UGainXEquipmentManagerComponent>();
}

void UGainXQuickBarComponent::EquipItemInSlot()
{
    check(Slots.IsValidIndex(ActiveSlotIndex));
    check(EquippedItem == nullptr);

    UGainXInventoryItem* SlotItem = Slots[ActiveSlotIndex];
    if (!SlotItem)
    {
        return;
    }

    const auto EquippableItemFragment = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>();
    if (!EquippableItemFragment)
    {
        return;
    }

    TSubclassOf<UGainXEquipmentObject> EquipObj = EquippableItemFragment->EquipmentObject;
    if (!EquipObj)
    {
        return;
    }

    UGainXEquipmentManagerComponent* EquipmentManager = FindEquipmentManagerComponent();
    if (!EquipmentManager)
    {
        return;
    }

    EquippedItem = EquipmentManager->EquipItem(EquipObj);
    if (EquippedItem)
    {
        EquippedItem->SetInstigator(SlotItem);
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
