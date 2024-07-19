// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GainXPlayerState.generated.h"

/**
 * Base player state class used by this project.
 */
UCLASS(Config = Game)
class GAINX_API AGainXPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AGainXPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~AActor interface
    virtual void PreInitializeComponents() override;
    virtual void PostInitializeComponents() override;
    //~End of AActor interface
};
