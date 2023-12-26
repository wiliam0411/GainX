// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GainXBaseWeapon.h"
#include "GainXLauncherWeapon.generated.h"

class AGainXProjectile;
class USoundCue;

UCLASS()
class GAINX_API AGainXLauncherWeapon : public AGainXBaseWeapon
{
	GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AGainXProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* NoAmmoSound;

    virtual void MakeShot() override;
};
