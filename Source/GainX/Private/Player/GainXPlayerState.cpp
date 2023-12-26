// GainX, All Rights Reserved

#include "Player/GainXPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXPlayerState, All, All);

void AGainXPlayerState::LogInfo()
{
	UE_LOG(LogGainXPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
