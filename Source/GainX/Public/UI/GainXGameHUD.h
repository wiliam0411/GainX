// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GainXCoreTypes.h"
#include "GainXGameHUD.generated.h"

class UGainXBaseWidget;

UCLASS()
class GAINX_API AGainXGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EGainXMatchState, UGainXBaseWidget*> GameWidgets;

    UPROPERTY()
    UGainXBaseWidget* CurrentWidget = nullptr;

    void DrawCrossHair();
    void OnMatchStateChanged(EGainXMatchState State);
};
