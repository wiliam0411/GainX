// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/GainXInventoryItemInstance.h"
#include "GainXInventoryItemFragment.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GAINX_API UGainXInventoryItemFragment : public UObject
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(UGainXInventoryItemDefinition* InventoryItem) const {}
};