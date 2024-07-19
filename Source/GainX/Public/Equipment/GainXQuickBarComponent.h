// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "GainXQuickBarComponent.generated.h"

class UGainXEquipmentManagerComponent;
class UGainXInventoryItem;
class UGainXEquipmentObject;

USTRUCT(BlueprintType)
struct FGainXQuickBarSlotsChangedMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    TObjectPtr<AActor> Owner = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = Inventory)
    TArray<TObjectPtr<UGainXInventoryItem>> Slots;
};

USTRUCT(BlueprintType)
struct FGainXQuickBarActiveIndexChangedMessage
{
    GENERATED_BODY()

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
    void CycleActiveSlotForward();

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    void CycleActiveSlotBackward();

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    void SetActiveSlotIndex(int32 NewIndex);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GainX|QuickBar")
    int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GainX|QuickBar")
    TArray<UGainXInventoryItem*> GetSlots() const { return Slots; }

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    void AddItemToSlot(int32 SlotIndex, UGainXInventoryItem* InventoryItem);

    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    UGainXInventoryItem* RemoveItemFromSlot(int32 SlotIndex);

    virtual void BeginPlay() override;

private:
    /**/
    UGainXEquipmentManagerComponent* FindEquipmentManagerComponent() const;

    /**/
    void EquipItemInSlot();

    /**/
    void UnequipItemInSlot();

    UPROPERTY()
    int32 NumSlots = 3;

    UPROPERTY()
    TArray<TObjectPtr<UGainXInventoryItem>> Slots;

    UPROPERTY()
    int32 ActiveSlotIndex = -1;

    UPROPERTY()
    TObjectPtr<UGainXEquipmentObject> EquippedItem;
};
