// GainX, All Rights Reserved

#include "Inventory/InventoryFragment_SetStats.h"
#include "Inventory/GainXInventoryItemDefinition.h"

void UInventoryFragment_SetStats::OnInstanceCreated(UGainXInventoryItemDefinition* InventoryItem) const 
{
    for (const FItemStats& ItemStats : InitialItemStats)
    {
        InventoryItem->AddItemStats(ItemStats.Tag, ItemStats.StackCount);
    }
}