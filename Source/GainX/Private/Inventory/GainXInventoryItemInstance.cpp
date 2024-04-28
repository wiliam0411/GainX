// GainX, All Rights Reserved

#include "Inventory/GainXInventoryItemInstance.h"
#include "Inventory/GainXInventoryItemDefinition.h"

UGainXInventoryItemInstance::UGainXInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

const UGainXInventoryItemFragment* UGainXInventoryItemInstance::FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const
{
    if (ItemDef && FragmentClass)
    {
        return GetDefault<UGainXInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
    }

    return nullptr;
}
