// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "GainXQuickBarComponent.generated.h"

class UGainXEquipmentManagerComponent;
class UGainXInventoryItem;
class AGainXEquipmentActor;
class UGainXAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FGainXQuickBarSlotsChangedMessage
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    TObjectPtr<AActor> Owner = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    TArray<TObjectPtr<UGainXInventoryItem>> Slots;
};

USTRUCT(BlueprintType)
struct FGainXQuickBarActiveIndexChangedMessage
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    TObjectPtr<AActor> Owner = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    int32 ActiveIndex = 0;
};

/**
 *
 */
UCLASS(BlueprintType)
class GAINX_API UGainXQuickBarComponent : public UControllerComponent
{
    GENERATED_BODY()

public:
    UGainXQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    int32 GetNextSlotIndex() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    int32 GetPreviousSlotIndex() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    int32 GetFirstFilledSlotIndex() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    int32 GetFirstFreeSlotIndex() const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GainX|QuickBar")
    TArray<UGainXInventoryItem*> GetSlots() const { return Slots; }

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    bool AddItemToSlot(int32 SlotIndex, UGainXInventoryItem* InventoryItem);

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    UGainXInventoryItem* RemoveItemFromSlot(int32 SlotIndex);

    void ChangeActiveSlotIndex(int32 NewIndex);

    // Spawns weapon actor and attaches it to the character
    void EquipItemInSlot();

    // Destroy weapon actor
    void UnequipItemInSlot();

    AGainXEquipmentActor* GetEquipmentActor() const { return EquippedItem; };

private:
    UGainXEquipmentManagerComponent* FindEquipmentManagerComponent() const;

    UPROPERTY()
    TArray<TObjectPtr<UGainXInventoryItem>> Slots;

    UPROPERTY()
    int32 ActiveSlotIndex = -1;

    UPROPERTY()
    TObjectPtr<AGainXEquipmentActor> EquippedItem;
};
