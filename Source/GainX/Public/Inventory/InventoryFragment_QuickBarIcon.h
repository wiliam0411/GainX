// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "Styling/SlateBrush.h"
#include "InventoryFragment_QuickBarIcon.generated.h"

UCLASS()
class GAINX_API UInventoryFragment_QuickBarIcon : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSlateBrush Brush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSlateBrush AmmoBrush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayNameWhenEquipped;
};
