// GainX, All Rights Reserved

#include "Inventory/GainXInventoryItem.h"

UGainXInventoryItem::UGainXInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

const UGainXInventoryItemFragment* UGainXInventoryItem::FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const
{
    if (!FragmentClass)
    {
        return nullptr;
    }

    for (UGainXInventoryItemFragment* Fragment : Fragments)
    {
        if (Fragment && Fragment->IsA(FragmentClass))
        {
            return Fragment;
        }
    }

    return nullptr;
}

void UGainXInventoryItem::AddItemStats(FGameplayTag Tag, int32 StackCount) 
{
    ItemStats.AddStats(Tag, StackCount);
}

void UGainXInventoryItem::RemoveItemStats(FGameplayTag Tag, int32 StackCount) 
{
    ItemStats.RemoveStats(Tag, StackCount);
}

int32 UGainXInventoryItem::GetItemStatsCount(FGameplayTag Tag) const
{
    return ItemStats.GetStatsCount(Tag);
}

bool UGainXInventoryItem::HasItemStats(FGameplayTag Tag) const
{
    return ItemStats.ContainsTag(Tag);
}
