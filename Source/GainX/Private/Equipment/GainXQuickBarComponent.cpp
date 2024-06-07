// GainX, All Rights Reserved

#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Equipment/GainXEquipmentObject.h"

UGainXQuickBarComponent::UGainXQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXQuickBarComponent::AddItemToSlot(int32 SlotIndex, UGainXInventoryItemDefinition* InventoryItem)
{
    if (Slots.IsValidIndex(SlotIndex) && (InventoryItem != nullptr))
    {
        if (Slots[SlotIndex] == nullptr)
        {
            Slots[SlotIndex] = InventoryItem;
        }
    }
}

UGainXInventoryItemDefinition* UGainXQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
    UGainXInventoryItemDefinition* Result = nullptr;

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

    UGainXInventoryItemDefinition* SlotItem = Slots[ActiveSlotIndex];
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
