// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/GainXInventoryItem.h"
#include "InventoryFragment_EquippableItem.generated.h"

class AGainXEquipmentActor;

UCLASS(Blueprintable)
class GAINX_API UInventoryFragment_EquippableItem : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AGainXEquipmentActor> EquipmentObject;
};
