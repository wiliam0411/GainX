// GainX, All Rights Reserved

#include "Inventory/GainXInventoryManagerComponent.h"
#include "Inventory/GainXInventoryItemDefinition.h"
#include "Inventory/GainXInventoryItemInstance.h"
#include "Inventory/GainXInventoryItemFragment.h"

UGainXInventoryManagerComponent::UGainXInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), InventoryList(this)
{
    PrimaryComponentTick.bCanEverTick = false;
}

UGainXInventoryItemInstance* UGainXInventoryManagerComponent::AddItemDefinition(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount)
{
    UGainXInventoryItemInstance* Result = nullptr;
    if (ItemDef != nullptr)
    {
        Result = InventoryList.AddEntry(ItemDef, StackCount);
    }
    return Result;
}

void UGainXInventoryManagerComponent::RemoveItemInstance(UGainXInventoryItemInstance* ItemInstance)
{
    InventoryList.RemoveEntry(ItemInstance);
}

TArray<UGainXInventoryItemInstance*> UGainXInventoryManagerComponent::GetAllItems() const
{
    return InventoryList.GetAllItems();
}

UGainXInventoryItemInstance* FGainXInventoryList::AddEntry(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount)
{
    UGainXInventoryItemInstance* Result = nullptr;

    FGainXInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.Instance = NewObject<UGainXInventoryItemInstance>(OwnerComponent->GetOwner());
    NewEntry.Instance->SetItemDef(ItemDef);
    for (UGainXInventoryItemFragment* Fragment : GetDefault<UGainXInventoryItemDefinition>(ItemDef)->Fragments)
    {
        if (Fragment)
        {
            Fragment->OnInstanceCreated(NewEntry.Instance);
        }
    }
    NewEntry.StackCount = StackCount;
    Result = NewEntry.Instance;

    return Result;
}

void FGainXInventoryList::RemoveEntry(UGainXInventoryItemInstance* Instance) 
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FGainXInventoryEntry& Entry = *EntryIt;
        if (Entry.Instance == Instance)
        {
            EntryIt.RemoveCurrent();
        }
    }
}

TArray<UGainXInventoryItemInstance*> FGainXInventoryList::GetAllItems() const
{
    TArray<UGainXInventoryItemInstance*> Results;
    Results.Reserve(Entries.Num());
    for (const FGainXInventoryEntry& Entry : Entries)
    {
        if (Entry.Instance != nullptr)
        {
            Results.Add(Entry.Instance);
        }
    }
    return Results;
}
