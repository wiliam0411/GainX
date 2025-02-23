// GainX, All Rights Reserved

#pragma once

#include "Components/ActorComponent.h"
#include "GainXInventoryManagerComponent.generated.h"

class UGainXInventoryItem;

/**
 * A single inventory entry
 */
USTRUCT(BlueprintType)
struct FGainXInventoryEntry
{
    GENERATED_BODY()

    FGainXInventoryEntry() {}

    friend FGainXInventoryList;
    friend UGainXInventoryManagerComponent;

private:
    UPROPERTY()
    TObjectPtr<UGainXInventoryItem> Item;

    UPROPERTY()
    int32 StackCount = 0;
};

/**
 * List of inventory items
 */
USTRUCT(BlueprintType)
struct FGainXInventoryList
{
    GENERATED_BODY()

public:
    FGainXInventoryList() : InventoryManager(nullptr) {}

    FGainXInventoryList(UActorComponent* InOwnerComponent) : InventoryManager(InOwnerComponent) {}

    /* Creates item instance and adds entry to InventoryEntries list */
    UGainXInventoryItem* AddEntry(TSubclassOf<UGainXInventoryItem> InventoryItemClass, int32 StackCount);

    /* Searches for item instance in InventoryEntries list and removes it */
    void RemoveEntry(UGainXInventoryItem* InventoryItem);

    /* Gathers all item instances */
    TArray<UGainXInventoryItem*> GetAllItems() const;

private:
    friend UGainXInventoryManagerComponent;

private:
    UPROPERTY()
    TArray<FGainXInventoryEntry> InventoryEntries;

    UPROPERTY()
    TObjectPtr<UActorComponent> InventoryManager;
};

/**
 * Manages an inventory
 */
UCLASS(BlueprintType)
class GAINX_API UGainXInventoryManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGainXInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
    UGainXInventoryItem* AddInventoryItem(TSubclassOf<UGainXInventoryItem> InventoryItemClass, int32 StackCount = 1);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
    void RemoveInventoryItem(UGainXInventoryItem* ItemInstance);

    UFUNCTION(BlueprintCallable, Category = Inventory)
    TArray<UGainXInventoryItem*> GetAllInventoryItems() const;

private:
    UPROPERTY()
    FGainXInventoryList InventoryList;
};
