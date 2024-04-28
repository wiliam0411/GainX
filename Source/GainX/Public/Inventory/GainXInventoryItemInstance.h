// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GainXInventoryItemInstance.generated.h"

class UGainXInventoryItemDefinition;
class UGainXInventoryItemFragment;

UCLASS()
class GAINX_API UGainXInventoryItemInstance : public UObject
{
    GENERATED_BODY()

public:
    UGainXInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void SetItemDef(TSubclassOf<UGainXInventoryItemDefinition> InDef) { ItemDef = InDef; }
    TSubclassOf<UGainXInventoryItemDefinition> GetItemDef() const { return ItemDef; }

    UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
    const UGainXInventoryItemFragment* FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const;

    template <typename ResultClass>
    const ResultClass* FindFragmentByClass() const
    {
        // TODO: Use Cast<>?
        return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
    }

private:
    UPROPERTY()
    TSubclassOf<UGainXInventoryItemDefinition> ItemDef;
};
