// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/GainXAbilitySet.h"
#include "GainXEquipmentManagerComponent.generated.h"

class UGainXAbilitySystemComponent;
class UGainXEquipmentObject;

/**
 * A single piece of applied equipment
 */
USTRUCT(BlueprintType)
struct FGainXEquipmentEntry
{
    GENERATED_BODY()

public:
    FGainXEquipmentEntry() {}

private:
    friend FGainXEquipmentList;
    friend UGainXEquipmentManagerComponent;

    UPROPERTY()
    TObjectPtr<UGainXEquipmentObject> EquipmentObject;

    UPROPERTY()
    FGainXAbilitySet_GrantedHandles GrantedHandles;
};

/**
 * List of applied equipment
 */
USTRUCT(BlueprintType)
struct FGainXEquipmentList
{
    GENERATED_BODY()

public:
    FGainXEquipmentList() : OwnerComponent(nullptr) {}

    FGainXEquipmentList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

    UGainXEquipmentObject* AddEntry(TSubclassOf<UGainXEquipmentObject> EquipmentObject);

    void RemoveEntry(UGainXEquipmentObject* EquipmentObject);

private:
    friend UGainXAbilitySystemComponent;
    friend UGainXEquipmentManagerComponent;

private:
    UPROPERTY()
    TArray<FGainXEquipmentEntry> EquipmentEntries;

    UPROPERTY()
    TObjectPtr<UActorComponent> OwnerComponent;

    UGainXAbilitySystemComponent* GetAbilitySystemComponent() const;
};

/**
 * Manages equipment applied to a pawn
 */
UCLASS()
class GAINX_API UGainXEquipmentManagerComponent : public UPawnComponent
{
    GENERATED_BODY()

public:
    UGainXEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /* Adds EquipmentObject to EquipmentList */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    UGainXEquipmentObject* EquipItem(TSubclassOf<UGainXEquipmentObject> EquipmentObject);

    /* Removes EquipmentObject from EquipmentList */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void UnequipItem(UGainXEquipmentObject* EquipmentObject);

    /* Returns the first equipped instance of a given type, or nullptr if none are found */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UGainXEquipmentObject* GetFirstInstanceOfType(TSubclassOf<UGainXEquipmentObject> InstanceType);

private:
    UPROPERTY()
    FGainXEquipmentList EquipmentList;
};
