// GainX, All Rights Reserved

#include "UI/GainXPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

void UGainXPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UGainXPauseWidget::OnClearPause);
    }
}

void UGainXPauseWidget::OnClearPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
