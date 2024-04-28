// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GainXEquipmentInstance.generated.h"

struct FGainXEquipmentActorToSpawn;

UCLASS(BlueprintType, Blueprintable)
class GAINX_API UGainXEquipmentInstance : public UObject
{
    GENERATED_BODY()

public:
    UGainXEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintPure, Category = Equipment)
    APawn* GetPawn() const;

    void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

    UFUNCTION(BlueprintPure, Category = Equipment)
    UObject* GetInstigator() const { return Instigator; }

    UFUNCTION(BlueprintPure, Category = Equipment)
    TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

    virtual void SpawnEquipmentActors(const TArray<FGainXEquipmentActorToSpawn>& ActorsToSpawn);
    virtual void DestroyEquipmentActors();

    virtual void OnEquipped();
    virtual void OnUnequipped();

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
    void K2_OnEquipped();

    UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
    void K2_OnUnequipped();

private:
    UPROPERTY()
    TObjectPtr<UObject> Instigator;

    UPROPERTY()
    TArray<TObjectPtr<AActor>> SpawnedActors;
};
