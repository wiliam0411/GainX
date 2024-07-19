// GainX, All Rights Reserved

#pragma once

#include "Engine/DataAsset.h"
#include "GainXPawnData.generated.h"

class APawn;
class UGainXAbilitySet;
class UGainXInputConfig;
class UGainXCameraMode;

/**
 * Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const)
class GAINX_API UGainXPawnData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UGainXPawnData(const FObjectInitializer& ObjectInitializer);

    /* Class to instantiate for this pawn (should usually derive from ALyraPawn or ALyraCharacter). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GainX|Pawn")
    TSubclassOf<APawn> PawnClass;

    /* Ability sets to grant to this pawn's ability system. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GainX|Abilities")
    TArray<TObjectPtr<UGainXAbilitySet>> AbilitySets;

    /* Input configuration used by player controlled pawns to create input mappings and bind input actions. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GainX|Input")
    TObjectPtr<UGainXInputConfig> InputConfig;

    /* Default camera mode used by player controlled pawns */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GainX|Camera")
    TSubclassOf<UGainXCameraMode> DefaultCameraMode;
};