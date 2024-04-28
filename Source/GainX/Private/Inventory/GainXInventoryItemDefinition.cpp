// GainX, All Rights Reserved

#include "Inventory/GainXInventoryItemDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GainXInventoryItemDefinition)

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
