// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GainXHUD.generated.h"

UCLASS(Config = Game, Meta = (DisplayName = "GainX HUD", Category = "GainX|HUD"))
class GAINX_API AGainXHUD : public AHUD
{
    GENERATED_BODY()

public:
    AGainXHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    //~UObject interface
    virtual void PreInitializeComponents() override;
    //~End of UObject interface

    //~AActor interface
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //~End of AActor interface
};
