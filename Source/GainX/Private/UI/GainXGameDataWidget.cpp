// GainX, All Rights Reserved

#include "UI/GainXGameDataWidget.h"
#include "GainXGameModeBase.h"
#include "Player/GainXPlayerState.h"

int32 UGainXGameDataWidget::GetCurrentRoundNum() const
{
    return 0;
}

int32 UGainXGameDataWidget::GetTotalRoundsNum() const
{
    return 0;
}

int32 UGainXGameDataWidget::GetRoundSecondRemaining() const
{
    return 0;
}

AGainXGameModeBase* UGainXGameDataWidget::GetGainXGameModeBase() const
{
    return GetWorld() ? Cast<AGainXGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AGainXPlayerState* UGainXGameDataWidget::GetGainXPlayerState() const
{
    return GetOwningPlayer() ? Cast<AGainXPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
