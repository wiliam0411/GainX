// GainX, All Rights Reserved

#include "UI/GainXActivatableWidget.h"

UGainXActivatableWidget::UGainXActivatableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

TOptional<FUIInputConfig> UGainXActivatableWidget::GetDesiredInputConfig() const
{
    switch (InputConfig)
    {
        case EGainXWidgetInputMode::GameAndMenu: return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
        case EGainXWidgetInputMode::Game: return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
        case EGainXWidgetInputMode::Menu: return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
        case EGainXWidgetInputMode::Default:
        default: return TOptional<FUIInputConfig>();
    }
}