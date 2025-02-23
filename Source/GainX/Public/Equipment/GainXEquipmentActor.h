// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GainXEquipmentActor.generated.h"

class UGainXInventoryItem;
class UGainXAbilitySet;

UCLASS()
class GAINX_API AGainXEquipmentActor : public AActor
{
    GENERATED_BODY()

public:
    AGainXEquipmentActor();

    UFUNCTION(BlueprintPure, Category = Equipment)
    UGainXInventoryItem* GetAssociatedInventoryItem() const { return AssociatedInventoryItem; }

    void SetAssociatedInventoryItem(UGainXInventoryItem* InventoryItem) { AssociatedInventoryItem = InventoryItem; }

    virtual void OnEquipped();
    virtual void OnUnequipped();

    UFUNCTION(BlueprintCallable, Category = Equipment)
    APawn* GetPawn() const;

    UFUNCTION(BlueprintCallable, Category = Equipment)
    UAnimMontage* GetAnimMontageByTag(FGameplayTag Tag) const;

    UFUNCTION(BlueprintCallable, Category = Equipment)
    UAnimSequence* GetAnimSequenceByTag(FGameplayTag Tag) const;

public:
    UPROPERTY(EditAnywhere, Category = Equipment)
    FName AttachSocket;

    UPROPERTY(EditAnywhere, Category = Equipment)
    FTransform AttachTransform;

    /* Gameplay ability sets to grant when this is equipped */
    UPROPERTY(EditDefaultsOnly, Category = "Equipment Object")
    TArray<TObjectPtr<const UGainXAbilitySet>> AbilitySetsToGrant;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
    void K2_OnEquipped();

    UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
    void K2_OnUnequipped();

private:
    UPROPERTY()
    TObjectPtr<UGainXInventoryItem> AssociatedInventoryItem;

    UPROPERTY(EditDefaultsOnly, Category = "Equipment Object")
    TMap<FGameplayTag, TObjectPtr<UAnimSequenceBase>> AnimSet;
};
