// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "InventoryFragment_SetStats.generated.h"

class ULyraInventoryItemInstance;

UCLASS()
class GAINX_API UInventoryFragment_SetStats : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(UGainXInventoryItemInstance* Instance) const override;

protected:
    UPROPERTY(EditDefaultsOnly)
    TMap<FGameplayTag, int32> InitialItemStats;
};
