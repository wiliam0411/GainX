// GainX, All Rights Reserved


#include "Components/GainXRespawnComponent.h"
#include "GainXGameModeBase.h"

UGainXRespawnComponent::UGainXRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGainXRespawnComponent::Respawn(int32 RespawnTime)
{
	if (!GetWorld()) return;

	RespawnCountdown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UGainXRespawnComponent::RespawnTimerUpdate, 1.0f, true);

}

bool UGainXRespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void UGainXRespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountdown == 0)
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<AGainXGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}

