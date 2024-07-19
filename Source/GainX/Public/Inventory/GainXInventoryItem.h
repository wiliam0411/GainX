// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/GainXInventoryItemFragment.h"
#include "Inventory/ItemStatsContainer.h"
#include "GainXInventoryItem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GAINX_API UGainXInventoryItem : public UObject
{
    GENERATED_BODY()

public:
    UGainXInventoryItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, Instanced)
    TArray<TObjectPtr<UGainXInventoryItemFragment>> Fragments;
    
public:
    /* Adds a specified number of stacks to the tag (does nothing if StackCount is below 1) */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GainX|Inventory")
    void AddItemStats(FGameplayTag Tag, int32 StackCount);

    /* Removes a specified number of stacks from the tag (does nothing if StackCount is below 1) */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GainX|Inventory")
    void RemoveItemStats(FGameplayTag Tag, int32 StackCount);

    /* Returns the stack count of the specified tag (or 0 if the tag is not present) */
    UFUNCTION(BlueprintCallable, Category = "GainX|Inventory")
    int32 GetItemStatsCount(FGameplayTag Tag) const;

    /* Returns true if there is at least one stack of the specified tag */
    UFUNCTION(BlueprintCallable, Category = "GainX|Inventory")
    bool HasItemStats(FGameplayTag Tag) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
    const UGainXInventoryItemFragment* FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const;

    template <typename T>
    const T* FindFragmentByClass() const
    {
        return (T*)FindFragmentByClass(T::StaticClass());
    }

private:
    UPROPERTY()
    FItemStatsContainer ItemStats;
};