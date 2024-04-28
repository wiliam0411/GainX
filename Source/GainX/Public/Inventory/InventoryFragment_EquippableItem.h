// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/GainXInventoryItemDefinition.h"
#include "Equipment/GainXEquipmentDefinition.h"
#include "InventoryFragment_EquippableItem.generated.h"

UCLASS()
class GAINX_API UInventoryFragment_EquippableItem : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UGainXEquipmentDefinition> EquipmentDefinition;
};
