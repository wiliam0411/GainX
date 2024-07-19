// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFeatureAction.h"
#include "GainXExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * Definition of a set of actions to perform as part of entering an experience
 */
UCLASS(BlueprintType, NotBlueprintable)
class GAINX_API UGainXExperienceActionSet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UGainXExperienceActionSet();

    /* List of actions to perform as this experience is loaded / activated / deactivated / unloaded */
    UPROPERTY(EditAnywhere, Instanced, Category = "Actions to Perform")
    TArray<TObjectPtr<UGameFeatureAction>> Actions;

    /* List of Game Feature Plugins this experience wants to have active */
    UPROPERTY(EditAnywhere, Category = "Feature Dependencies")
    TArray<FString> GameFeaturesToEnable;
};
