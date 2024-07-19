// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GainXActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class EGainXWidgetInputMode : uint8
{
    Default,
    GameAndMenu,
    Game,
    Menu
};

/**
 * Activatable widget that automatically drives the desired input config when activated
 */
UCLASS(Abstract, Blueprintable)
class GAINX_API UGainXActivatableWidget : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    UGainXActivatableWidget(const FObjectInitializer& ObjectInitializer);

    //~UCommonActivatableWidget interface
    virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
    //~End of UCommonActivatableWidget interface

protected:
    /* The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
    UPROPERTY(EditDefaultsOnly, Category = Input)
    EGainXWidgetInputMode InputConfig = EGainXWidgetInputMode::Default;

    /* The desired mouse behavior when the game gets input */
    UPROPERTY(EditDefaultsOnly, Category = Input)
    EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
