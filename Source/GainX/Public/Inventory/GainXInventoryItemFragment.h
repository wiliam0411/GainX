// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GainXInventoryItemFragment.generated.h"

class UGainXInventoryItem;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GAINX_API UGainXInventoryItemFragment : public UObject
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(UGainXInventoryItem* InventoryItem) const {}
};