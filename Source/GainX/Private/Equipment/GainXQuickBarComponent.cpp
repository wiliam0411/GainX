// GainX, All Rights Reserved

#include "Equipment/GainXQuickBarComponent.h"
#include "Equipment/GainXEquipmentInstance.h"
#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Inventory/InventoryFragment_EquippableItem.h"

UGainXQuickBarComponent::UGainXQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXQuickBarComponent::AddItemToSlot(int32 SlotIndex, UGainXInventoryItemInstance* Item)
{
    if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
    {
        if (Slots[SlotIndex] == nullptr)
        {
            Slots[SlotIndex] = Item;
        }
    }
}

UGainXInventoryItemInstance* UGainXQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
    UGainXInventoryItemInstance* Result = nullptr;

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

UGainXEquipmentManagerComponent* UGainXQuickBarComponent::FindEquipmentManager() const
{
    if (AController* OwnerController = Cast<AController>(GetOwner()))
    {
        if (APawn* Pawn = OwnerController->GetPawn())
        {
            return Pawn->FindComponentByClass<UGainXEquipmentManagerComponent>();
        }
    }
    return nullptr;
}

void UGainXQuickBarComponent::EquipItemInSlot()
{
    check(Slots.IsValidIndex(ActiveSlotIndex));
    check(EquippedItem == nullptr);

    if (UGainXInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
    {
        if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
        {
            TSubclassOf<UGainXEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
            if (EquipDef != nullptr)
            {
                if (UGainXEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
                {
                    EquippedItem = EquipmentManager->EquipItem(EquipDef);
                    if (EquippedItem != nullptr)
                    {
                        EquippedItem->SetInstigator(SlotItem);
                    }
                }
            }
        }
    }
}

void UGainXQuickBarComponent::UnequipItemInSlot()
{
    if (UGainXEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
    {
        if (EquippedItem != nullptr)
        {
            EquipmentManager->UnequipItem(EquippedItem);
            EquippedItem = nullptr;
        }
    }
}
