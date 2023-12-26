// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/GainXBasePickup.h"
#include "GainXAmmoPickup.generated.h"

class AGainXBaseWeapon;

UCLASS()
class GAINX_API AGainXAmmoPickup : public AGainXBasePickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    int32 ClipsAmount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TSubclassOf<AGainXBaseWeapon> WeaponType;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
