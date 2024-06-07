// GainX, All Rights Reserved

#include "Inventory/GainXInventoryItemDefinition.h"

UGainXInventoryItemDefinition::UGainXInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

const UGainXInventoryItemFragment* UGainXInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const
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

void UGainXInventoryItemDefinition::AddItemStats(FGameplayTag Tag, int32 StackCount) 
{
    ItemStats.AddStats(Tag, StackCount);
}

void UGainXInventoryItemDefinition::RemoveItemStats(FGameplayTag Tag, int32 StackCount) 
{
    ItemStats.RemoveStats(Tag, StackCount);
}

int32 UGainXInventoryItemDefinition::GetItemStatsCount(FGameplayTag Tag) const
{
    return ItemStats.GetStatsCount(Tag);
}

bool UGainXInventoryItemDefinition::HasItemStats(FGameplayTag Tag) const
{
    return ItemStats.ContainsTag(Tag);
}
