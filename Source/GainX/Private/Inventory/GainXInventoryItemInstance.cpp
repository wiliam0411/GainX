// GainX, All Rights Reserved

#include "Inventory/GainXInventoryItemInstance.h"
#include "Inventory/GainXInventoryItemDefinition.h"

UGainXInventoryItemInstance::UGainXInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXInventoryItemInstance::AddItemStats(FGameplayTag Tag, int32 StackCount) 
{
    ItemStats.AddStats(Tag, StackCount);
}

void UGainXInventoryItemInstance::RemoveItemStats(FGameplayTag Tag, int32 StackCount) 
{
    ItemStats.RemoveStats(Tag, StackCount);
}

int32 UGainXInventoryItemInstance::GetItemStatsCount(FGameplayTag Tag) const
{
    return ItemStats.GetStatsCount(Tag);
}

bool UGainXInventoryItemInstance::HasItemStats(FGameplayTag Tag) const
{
    return ItemStats.ContainsTag(Tag);
}

const UGainXInventoryItemFragment* UGainXInventoryItemInstance::FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const
{
    if (ItemDef && FragmentClass)
    {
        return GetDefault<UGainXInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
    }

    return nullptr;
}
