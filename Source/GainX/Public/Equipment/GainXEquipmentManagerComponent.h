// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/GainXAbilitySet.h"
#include "GainXEquipmentManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipItem, AGainXEquipmentActor*, EquipmentActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnequipItem);

class UGainXAbilitySystemComponent;
class AGainXEquipmentActor;

USTRUCT(BlueprintType)
struct FGainXEquipmentMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UGainXEquipmentManagerComponent> EquipmentManagerComponent = nullptr;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AGainXEquipmentActor> EquipmentActor = nullptr;
};

/**
 * A single piece of applied equipment
 */
USTRUCT(BlueprintType)
struct FGainXEquipmentEntry
{
    GENERATED_BODY()

public:
    FGainXEquipmentEntry() {}

    friend FGainXEquipmentList;
    friend UGainXEquipmentManagerComponent;

private:
    UPROPERTY()
    TObjectPtr<AGainXEquipmentActor> Item;

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
    FGainXEquipmentList() : EquipmentManager(nullptr) {}

    FGainXEquipmentList(UActorComponent* InOwnerComponent) : EquipmentManager(InOwnerComponent) {}

    AGainXEquipmentActor* AddEntry(TSubclassOf<AGainXEquipmentActor> EquipmentItemClass);

    void RemoveEntry(AGainXEquipmentActor* EquipmentItem);

private:
    friend UGainXAbilitySystemComponent;
    friend UGainXEquipmentManagerComponent;

private:
    UPROPERTY()
    TArray<FGainXEquipmentEntry> EquipmentEntries;

    /* Need to store this in order to get owner of the EquipmentManagerComponent */
    UPROPERTY()
    TObjectPtr<UActorComponent> EquipmentManager;

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

    void BeginPlay() override;

    /* Adds EquipmentObject to EquipmentList */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    AGainXEquipmentActor* EquipItem(TSubclassOf<AGainXEquipmentActor> EquipmentItemClass, UGainXInventoryItem* AssociatedInventoryItem = nullptr);

    /* Removes EquipmentObject from EquipmentList */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void UnequipItem(AGainXEquipmentActor* EquipmentItem);

    /* Returns the first equipped instance of a given type, or nullptr if none are found */
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "EquipmentActorClass"))
    AGainXEquipmentActor* GetFirstInstanceOfType(TSubclassOf<AGainXEquipmentActor> EquipmentActorClass);

    UPROPERTY(BlueprintAssignable)
    FOnEquipItem OnEquipItem;

    UPROPERTY(BlueprintAssignable)
    FOnUnequipItem OnUnequipItem;

private:
    void BroadcastEquipmentMessage(FGameplayTag Channel, AGainXEquipmentActor* EquipmentActor);

private:
    UPROPERTY()
    FGainXEquipmentList EquipmentList;
};
