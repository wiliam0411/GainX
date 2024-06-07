// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GainXBaseWeapon.h"
#include "GainXRifleWeapon.generated.h"

class UGainXWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class GAINX_API AGainXRifleWeapon : public AGainXBaseWeapon
{
	GENERATED_BODY()

public:
    AGainXRifleWeapon();
};
