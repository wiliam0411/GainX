// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/GainXInventoryItem.h"
#include "Equipment/GainXEquipmentObject.h"
#include "InventoryFragment_EquippableItem.generated.h"

UCLASS(Blueprintable)
class GAINX_API UInventoryFragment_EquippableItem : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGainXEquipmentObject> EquipmentObject;
};
