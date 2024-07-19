// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "Inventory/ItemStatsContainer.h"
#include "InventoryFragment_SetStats.generated.h"

class UGainXInventoryItem;

UCLASS()
class GAINX_API UInventoryFragment_SetStats : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(UGainXInventoryItem* InventoryItem) const override;

protected:
    UPROPERTY(EditDefaultsOnly)
    TArray<FItemStats> InitialItemStats;
};