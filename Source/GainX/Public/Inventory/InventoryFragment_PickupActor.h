// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "InventoryFragment_PickupActor.generated.h"

class AGainXPickupActor;

/**
 *
 */
UCLASS(Blueprintable)
class GAINX_API UInventoryFragment_PickupActor : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AGainXPickupActor> PickupActor;
};
