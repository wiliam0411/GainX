// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GainXExperience.generated.h"

class UGainXPawnData;
class UGameFeatureAction;
class UGainXExperienceActionSet;

/**
 * Definition of an experience
 */
UCLASS(BlueprintType, Const)
class GAINX_API UGainXExperience : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UGainXExperience();

    /* List of Game Feature Plugins this experience wants to have active */
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TArray<FString> GameFeaturesToEnable;

    /* The default pawn class to spawn for players */
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TObjectPtr<const UGainXPawnData> DefaultPawnData;

    // List of actions to perform as this experience is loaded/activated/deactivated/unloaded
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
    TArray<TObjectPtr<UGameFeatureAction>> Actions;

    // List of additional action sets to compose into this experience
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TArray<TObjectPtr<UGainXExperienceActionSet>> ActionSets;
};