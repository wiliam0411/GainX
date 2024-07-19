// GainX, All Rights Reserved

#include "Inventory/GainXInventoryManagerComponent.h"
#include "Inventory/GainXInventoryItem.h"
#include "Inventory/GainXInventoryItemFragment.h"

UGainXInventoryManagerComponent::UGainXInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), InventoryList(this)
{
    PrimaryComponentTick.bCanEverTick = false;
}

UGainXInventoryItem* UGainXInventoryManagerComponent::AddInventoryItem(TSubclassOf<UGainXInventoryItem> InventoryItemClass, int32 StackCount)
{
    return InventoryList.AddEntry(InventoryItemClass, StackCount);
}

void UGainXInventoryManagerComponent::RemoveInventoryItem(UGainXInventoryItem* ItemInstance)
{
    InventoryList.RemoveEntry(ItemInstance);
}

TArray<UGainXInventoryItem*> UGainXInventoryManagerComponent::GetAllInventoryItems() const
{
    return InventoryList.GetAllItems();
}

UGainXInventoryItem* FGainXInventoryList::AddEntry(TSubclassOf<UGainXInventoryItem> InventoryItemClass, int32 StackCount)
{
    const auto InventoryItemCDO = GetDefault<UGainXInventoryItem>(InventoryItemClass);

    FGainXInventoryEntry& NewEntry = InventoryEntries.AddDefaulted_GetRef();
    NewEntry.Item = NewObject<UGainXInventoryItem>(OwnerComponent->GetOwner(), InventoryItemClass);
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

void FGainXInventoryList::RemoveEntry(UGainXInventoryItem* InventoryItem)
{
    for (auto EntryIt = InventoryEntries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FGainXInventoryEntry& Entry = *EntryIt;
        if (Entry.Item == InventoryItem)
        {
            EntryIt.RemoveCurrent();
        }
    }
}

TArray<UGainXInventoryItem*> FGainXInventoryList::GetAllItems() const
{
    TArray<UGainXInventoryItem*> Results;
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