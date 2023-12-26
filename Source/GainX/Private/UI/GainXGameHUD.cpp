// GainX, All Rights Reserved

#include "UI/GainXGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/GainXBaseWidget.h"
#include "GainXGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXGameHUD, All, All);

void AGainXGameHUD::DrawHUD()
{
    Super::DrawHUD();

    // DrawCrossHair();
}

void AGainXGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(EGainXMatchState::InProgress, CreateWidget<UGainXBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(EGainXMatchState::Pause, CreateWidget<UGainXBaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EGainXMatchState::GameOver, CreateWidget<UGainXBaseWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<AGainXGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &AGainXGameHUD::OnMatchStateChanged);
        }
    }
}

void AGainXGameHUD::OnMatchStateChanged(EGainXMatchState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }

    UE_LOG(LogGainXGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void AGainXGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
