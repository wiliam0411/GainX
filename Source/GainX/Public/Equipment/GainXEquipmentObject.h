// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GainXEquipmentObject.generated.h"

class UGainXAbilitySet;

/**
 * Struct for attaching blueprint actors to character
 */
USTRUCT()
struct FGainXEquipmentActorSpawnConfig
{
    GENERATED_BODY()

public:
    FGainXEquipmentActorSpawnConfig() {}

    UPROPERTY(EditAnywhere, Category = Equipment)
    TSubclassOf<AActor> Actor;

    UPROPERTY(EditAnywhere, Category = Equipment)
    FName AttachSocket;

    UPROPERTY(EditAnywhere, Category = Equipment)
    FTransform AttachTransform;
};

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class GAINX_API UGainXEquipmentObject : public UObject
{
    GENERATED_BODY()

public:
    UGainXEquipmentObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintPure, Category = Equipment)
    APawn* GetPawn() const;

    UFUNCTION(BlueprintPure, Category = Equipment)
    TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

    UFUNCTION(BlueprintPure, Category = Equipment)
    UObject* GetInstigator() const { return Instigator; }

    void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

    virtual void OnEquipped();
    virtual void OnUnequipped();

    virtual void SpawnEquipmentActors();
    virtual void DestroyEquipmentActors();

public:
    /* Gameplay ability sets to grant when this is equipped */
    UPROPERTY(EditDefaultsOnly, Category = "Equipment Object", meta = (DisplayPriority = 1))
    TArray<TObjectPtr<const UGainXAbilitySet>> AbilitySetsToGrant;

    /* Actors to spawn on the pawn when this is equipped */
    UPROPERTY(EditDefaultsOnly, Category = "Equipment Object", meta = (DisplayPriority = 2))
    TArray<FGainXEquipmentActorSpawnConfig> ActorsToSpawn;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
    void K2_OnEquipped();

    UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
    void K2_OnUnequipped();

private:
    UPROPERTY()
    TArray<TObjectPtr<AActor>> SpawnedActors;

    UPROPERTY()
    TObjectPtr<UObject> Instigator;
};
