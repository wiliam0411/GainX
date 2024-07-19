// GainX, All Rights Reserved

#include "Inventory/InventoryFragment_SetStats.h"
#include "Inventory/GainXInventoryItem.h"

void UInventoryFragment_SetStats::OnInstanceCreated(UGainXInventoryItem* InventoryItem) const 
{
    for (const FItemStats& ItemStats : InitialItemStats)
    {
        InventoryItem->AddItemStats(ItemStats.Tag, ItemStats.StackCount);
    }
}