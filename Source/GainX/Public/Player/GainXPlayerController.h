// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GainXCoreTypes.h"
#include "GainXPlayerController.generated.h"

class UGainXRespawnComponent;

UCLASS()
class GAINX_API AGainXPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AGainXPlayerController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    UGainXRespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;

private:
    void OnPauseGame();
    void OnMatchStateChanged(EGainXMatchState State);
    void OnMuteSound();
};