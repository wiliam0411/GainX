// GainX, All Rights Reserved


#include "Inventory/InventoryFragment_SetStats.h"
#include "Inventory/GainXInventoryItemInstance.h"

void UInventoryFragment_SetStats::OnInstanceCreated(UGainXInventoryItemInstance* Instance) const 
{
    for (const auto KVP : InitialItemStats)
    {
        // TODO: Implement
    }
}
