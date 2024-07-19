// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "GainXUIManagerSubsystem.generated.h"

/**
 *
 */
UCLASS()
class GAINX_API UGainXUIManagerSubsystem : public UGameUIManagerSubsystem
{
    GENERATED_BODY()

public:
    UGainXUIManagerSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
};
