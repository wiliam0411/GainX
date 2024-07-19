// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "GainXGameState.generated.h"

class UGainXExperienceManagerComponent;

/**
 * The base game state class used by this project.
 */
UCLASS(Config = Game)
class GAINX_API AGainXGameState : public AModularGameStateBase
{
    GENERATED_BODY()

public:
    AGainXGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
    // Handles loading and managing the current gameplay experience
    UPROPERTY()
    TObjectPtr<UGainXExperienceManagerComponent> ExperienceManagerComponent;
};
