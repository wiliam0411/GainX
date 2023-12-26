// GainX, All Rights Reserved

#include "UI/GainXSpectatorWidget.h"
#include "GainXUtils.h"
#include "Components/GainXRespawnComponent.h"

bool UGainXSpectatorWidget::GetRespawnTime(int32& CountdownTime) const
{
    const auto RespawnComponent = GainXUtils::GetGainXPlayerComponent<UGainXRespawnComponent>(GetOwningPlayer());
    if (!RespawnComponent && RespawnComponent->IsRespawnInProgress()) return false;

    CountdownTime = RespawnComponent->GetRespawnCountdown();
    return true;
}
