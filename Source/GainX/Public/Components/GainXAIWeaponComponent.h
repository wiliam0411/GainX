// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/GainXWeaponComponent.h"
#include "GainXAIWeaponComponent.generated.h"

UCLASS()
class GAINX_API UGainXAIWeaponComponent : public UGainXWeaponComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;
};
