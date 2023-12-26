// GainX, All Rights Reserved

#include "UI/GainXPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UGainXPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
    if (!PlayerNameTextBlock) return;
    PlayerNameTextBlock->SetText(Text);
}

void UGainXPlayerStatRowWidget::SetKills(const FText& Text)
{
    if (!KillsTextBlock) return;
    KillsTextBlock->SetText(Text);
}

void UGainXPlayerStatRowWidget::SetDeaths(const FText& Text)
{
    if (!DeathsTextBlock) return;
    DeathsTextBlock->SetText(Text);
}

void UGainXPlayerStatRowWidget::SetTeam(const FText& Text)
{
    if (!TeamTextBlock) return;
    TeamTextBlock->SetText(Text);
}

void UGainXPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
    if (!PlayerIndicatorImage) return;
    PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGainXPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
    if (!TeamImage) return;
    TeamImage->SetColorAndOpacity(Color);
}
