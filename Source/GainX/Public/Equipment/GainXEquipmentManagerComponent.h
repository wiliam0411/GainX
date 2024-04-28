// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GainXEquipmentManagerComponent.generated.h"

class UGainXAbilitySystemComponent;
class UGainXEquipmentDefinition;
class UGainXEquipmentInstance;

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
    TSubclassOf<UGainXEquipmentDefinition> EquipmentDefinition;

    UPROPERTY()
    TObjectPtr<UGainXEquipmentInstance> Instance = nullptr;
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

    UGainXEquipmentInstance* AddEntry(TSubclassOf<UGainXEquipmentDefinition> EquipmentDef);

    void RemoveEntry(UGainXEquipmentInstance* Instance);

private:
    UGainXAbilitySystemComponent* GetAbilitySystemComponent() const;

    friend UGainXAbilitySystemComponent;
    friend UGainXEquipmentManagerComponent;

private:
    UPROPERTY()
    TArray<FGainXEquipmentEntry> Entries;

    UPROPERTY()
    TObjectPtr<UActorComponent> OwnerComponent;
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

    /**/
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    UGainXEquipmentInstance* EquipItem(TSubclassOf<UGainXEquipmentDefinition> EquipmentDef);

    /**/
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void UnequipItem(UGainXEquipmentInstance* ItemInstance);

    /** Returns the first equipped instance of a given type, or nullptr if none are found */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UGainXEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UGainXEquipmentInstance> InstanceType);

private:
    UPROPERTY()
    FGainXEquipmentList EquipmentList;
};
