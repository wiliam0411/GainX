// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GainXEquipmentDefinition.generated.h"

class UGainXEquipmentInstance;
class UGainXAbilitySet;

USTRUCT()
struct FGainXEquipmentActorToSpawn
{
    GENERATED_BODY()

    FGainXEquipmentActorToSpawn() {}

    UPROPERTY(EditAnywhere, Category = Equipment)
    TSubclassOf<AActor> ActorToSpawn;

    UPROPERTY(EditAnywhere, Category = Equipment)
    FName AttachSocket;

    UPROPERTY(EditAnywhere, Category = Equipment)
    FTransform AttachTransform;
};

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class GAINX_API UGainXEquipmentDefinition : public UObject
{
    GENERATED_BODY()

public:
    UGainXEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditDefaultsOnly, Category = Equipment)
    TSubclassOf<UGainXEquipmentInstance> InstanceType;

    UPROPERTY(EditDefaultsOnly, Category = Equipment)
    TArray<TObjectPtr<const UGainXAbilitySet>> AbilitySetsToGrant;

    UPROPERTY(EditDefaultsOnly, Category = Equipment)
    TArray<FGainXEquipmentActorToSpawn> ActorsToSpawn;
};
