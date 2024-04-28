// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "GainXQuickBarComponent.generated.h"

class UGainXEquipmentManagerComponent;
class UGainXInventoryItemInstance;
class UGainXEquipmentInstance;

/**
 *
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GAINX_API UGainXQuickBarComponent : public UControllerComponent
{
    GENERATED_BODY()

public:
    UGainXQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /**/
    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    void SetActiveSlotIndex(int32 NewIndex);

    /**/
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GainX|QuickBar")
    int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

    /**/
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GainX|QuickBar")
    TArray<UGainXInventoryItemInstance*> GetSlots() const { return Slots; }

    /**/
    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    void AddItemToSlot(int32 SlotIndex, UGainXInventoryItemInstance* Item);

    /**/
    UFUNCTION(BlueprintCallable, Category = "GainX|QuickBar")
    UGainXInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

    virtual void BeginPlay() override;

private:
    /**/
    UGainXEquipmentManagerComponent* FindEquipmentManager() const;

    /**/
    void EquipItemInSlot();

    /**/
    void UnequipItemInSlot();

    UPROPERTY()
    int32 NumSlots = 3;

    UPROPERTY()
    TArray<TObjectPtr<UGainXInventoryItemInstance>> Slots;

    UPROPERTY()
    int32 ActiveSlotIndex = -1;

    UPROPERTY()
    TObjectPtr<UGainXEquipmentInstance> EquippedItem;
};
