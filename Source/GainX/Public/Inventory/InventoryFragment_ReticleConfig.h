// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "Blueprint/UserWidget.h"
#include "InventoryFragment_ReticleConfig.generated.h"

UCLASS()
class GAINX_API UInventoryFragment_ReticleConfig : public UGainXInventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
    TArray<TSubclassOf<UUserWidget>> ReticleWidgets;
};