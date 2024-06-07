// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/ItemStatsContainer.h"
#include "GainXInventoryItemInstance.generated.h"

class UGainXInventoryItemDefinition;
class UGainXInventoryItemFragment;

UCLASS(BlueprintType)
class GAINX_API UGainXInventoryItemInstance : public UObject
{
    GENERATED_BODY()

public:
    UGainXInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Adds a specified number of stacks to the tag(does nothing if StackCount is below 1) */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GainX|Inventory")
    void AddItemStats(FGameplayTag Tag, int32 StackCount);

    /** Removes a specified number of stacks from the tag (does nothing if StackCount is below 1) */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GainX|Inventory")
    void RemoveItemStats(FGameplayTag Tag, int32 StackCount);

    /** Returns the stack count of the specified tag (or 0 if the tag is not present) */
    UFUNCTION(BlueprintCallable, Category = "GainX|Inventory")
    int32 GetItemStatsCount(FGameplayTag Tag) const;

    /** Returns true if there is at least one stack of the specified tag */
    UFUNCTION(BlueprintCallable, Category = "GainX|Inventory")
    bool HasItemStats(FGameplayTag Tag) const;

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

    UPROPERTY()
    FItemStatsContainer ItemStats;
};
