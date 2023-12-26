// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "GainXAIPerceptionComponent.generated.h"

UCLASS()
class GAINX_API UGainXAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
};
