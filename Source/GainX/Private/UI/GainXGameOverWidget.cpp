// GainX, All Rights Reserved

#include "UI/GainXGameOverWidget.h"
#include "GainXGameModeBase.h"
#include "Player/GainXPlayerState.h"
#include "UI/GainXPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GainXUtils.h"

void UGainXGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &UGainXGameOverWidget::OnResetLevel);
    }
}

void UGainXGameOverWidget::OnMatchStateChanged(EGainXMatchState State)
{
    if (State == EGainXMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void UGainXGameOverWidget::UpdatePlayerStat()
{
    if (!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AGainXPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        const auto PlayerStatRowWidget = CreateWidget<UGainXPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void UGainXGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
