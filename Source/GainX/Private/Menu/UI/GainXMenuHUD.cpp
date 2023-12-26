// GainX, All Rights Reserved

#include "Menu/UI/GainXMenuHUD.h"
#include "UI/GainXBaseWidget.h"

void AGainXMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (const auto MenuWidget = CreateWidget<UGainXBaseWidget>(GetWorld(), MenuWidgetClass))
    {
        MenuWidget->AddToViewport();
        MenuWidget->Show();
    }
}
