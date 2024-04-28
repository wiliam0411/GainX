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
    TObjectPtr<UGainXInventoryItemInstance> Instance = nullptr;

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

    /**/
    UGainXInventoryItemInstance* AddEntry(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount);
    
    /**/
    void RemoveEntry(UGainXInventoryItemInstance* Instance);

    /**/
    TArray<UGainXInventoryItemInstance*> GetAllItems() const;

private:
    friend UGainXInventoryManagerComponent;

private:
    UPROPERTY()
    TArray<FGainXInventoryEntry> Entries;

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
    UGainXInventoryItemInstance* AddItemDefinition(TSubclassOf<UGainXInventoryItemDefinition> ItemDef, int32 StackCount = 1);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
    void RemoveItemInstance(UGainXInventoryItemInstance* ItemInstance);

    UFUNCTION(BlueprintCallable, Category = Inventory, BlueprintPure = false)
    TArray<UGainXInventoryItemInstance*> GetAllItems() const;

private:
    UPROPERTY()
    FGainXInventoryList InventoryList;
};
