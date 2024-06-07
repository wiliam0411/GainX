// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GainXInventoryManagerComponent.generated.h"

class UGainXInventoryItemInstance;
class UGainXInventoryItemDefinition;

/**
 * A single entry in an inventory
 */
USTRUCT(BlueprintType)
struct FGainXInventoryEntry
{
    GENERATED_BODY()

    FGainXInventoryEntry() {}

private:
    friend FGainXInventoryList;
    friend UGainXInventoryManagerComponent;

private:
    UPROPERTY()
    TObjectPtr<UGainXInventoryItemDefinition> Item;

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
    FGainXInventoryList() : OwnerComponent(nullptr) {}

    FGainXInventoryList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

    UGainXInventoryItemDefinition* AddEntry(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount);

    void RemoveEntry(UGainXInventoryItemDefinition* Instance);

    TArray<UGainXInventoryItemDefinition*> GetAllItems() const;

private:
    friend UGainXInventoryManagerComponent;

private:
    UPROPERTY()
    TArray<FGainXInventoryEntry> InventoryEntries;

    UPROPERTY()
    TObjectPtr<UActorComponent> OwnerComponent;
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
    UGainXInventoryItemDefinition* AddInventoryItem(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount = 1);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
    void RemoveInventoryItem(UGainXInventoryItemDefinition* ItemInstance);

    UFUNCTION(BlueprintCallable, Category = Inventory)
    TArray<UGainXInventoryItemDefinition*> GetAllInventoryItems() const;

private:
    UPROPERTY()
    FGainXInventoryList InventoryList;
};
