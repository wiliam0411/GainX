// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GainXRespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAINX_API UGainXRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGainXRespawnComponent();

	void Respawn(int32 RespawnTime);

	int32 GetRespawnCountdown() const { return RespawnCountdown; }

	bool IsRespawnInProgress() const;

private:
	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountdown = 0;

	void RespawnTimerUpdate();
};
