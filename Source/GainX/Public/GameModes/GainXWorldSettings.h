// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "GainXWorldSettings.generated.h"

class UGainXExperience;

/**
 * The default world settings object, used primarily to set the default gameplay experience to use when playing on this map
 */
UCLASS()
class GAINX_API AGainXWorldSettings : public AWorldSettings
{
    GENERATED_BODY()

public:
    AGainXWorldSettings(const FObjectInitializer& ObjectInitializer);

    /* Returns the default experience if it is not overridden by the user - facing experience */
    FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
    /*The default experience if it is not overridden by the user - facing experience */
    UPROPERTY(EditDefaultsOnly, Category = GameMode)
    TSoftClassPtr<UGainXExperience> DefaultGameplayExperience;
};