// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "GainXInventoryItemDefinition.generated.h"

UCLASS(Blueprintable, Const, Abstract)
class GAINX_API UGainXInventoryItemDefinition : public UObject
{
    GENERATED_BODY()

public:
    UGainXInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, Instanced)
    TArray<TObjectPtr<UGainXInventoryItemFragment>> Fragments;

    const UGainXInventoryItemFragment* FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const;
};