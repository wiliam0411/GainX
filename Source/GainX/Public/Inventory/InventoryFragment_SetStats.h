// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "Inventory/ItemStatsContainer.h"
#include "InventoryFragment_SetStats.generated.h"

class UGainXInventoryItemDefinition;

UCLASS()
class GAINX_API UInventoryFragment_SetStats : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(UGainXInventoryItemDefinition* InventoryItem) const override;

protected:
    UPROPERTY(EditDefaultsOnly)
    TArray<FItemStats> InitialItemStats;
};