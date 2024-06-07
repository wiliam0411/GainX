// GainX, All Rights Reserved

#include "Inventory/GainXInventoryManagerComponent.h"
#include "Inventory/GainXInventoryItemDefinition.h"
#include "Inventory/GainXInventoryItemInstance.h"
#include "Inventory/GainXInventoryItemFragment.h"

UGainXInventoryManagerComponent::UGainXInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), InventoryList(this)
{
    PrimaryComponentTick.bCanEverTick = false;
}

UGainXInventoryItemDefinition* UGainXInventoryManagerComponent::AddInventoryItem(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount)
{
    return InventoryList.AddEntry(ItemDef, StackCount);
}

void UGainXInventoryManagerComponent::RemoveInventoryItem(UGainXInventoryItemDefinition* ItemInstance) 
{
    InventoryList.RemoveEntry(ItemInstance);
}

TArray<UGainXInventoryItemDefinition*> UGainXInventoryManagerComponent::GetAllInventoryItems() const
{
    return InventoryList.GetAllItems();
}

UGainXInventoryItemDefinition* FGainXInventoryList::AddEntry(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount)
{
    const auto InventoryItemCDO = GetDefault<UGainXInventoryItemDefinition>(ItemDef);

    FGainXInventoryEntry& NewEntry = InventoryEntries.AddDefaulted_GetRef();
    NewEntry.Item = NewObject<UGainXInventoryItemDefinition>(OwnerComponent->GetOwner(), ItemDef);
    NewEntry.StackCount = StackCount;

    for (UGainXInventoryItemFragment* Fragment : InventoryItemCDO->Fragments)
    {
        if (Fragment)
        {
            Fragment->OnInstanceCreated(NewEntry.Item);
        }
    }

    return NewEntry.Item;
}

void FGainXInventoryList::RemoveEntry(UGainXInventoryItemDefinition* Instance)
{
    for (auto EntryIt = InventoryEntries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FGainXInventoryEntry& Entry = *EntryIt;
        if (Entry.Item == Instance)
        {
            EntryIt.RemoveCurrent();
        }
    }
}

TArray<UGainXInventoryItemDefinition*> FGainXInventoryList::GetAllItems() const
{
    TArray<UGainXInventoryItemDefinition*> Results;
    Results.Reserve(InventoryEntries.Num());
    for (const FGainXInventoryEntry& Entry : InventoryEntries)
    {
        if (Entry.Item)
        {
            Results.Add(Entry.Item);
        }
    }
    return Results;
}