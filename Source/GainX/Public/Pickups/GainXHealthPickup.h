// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/GainXBasePickup.h"
#include "GainXHealthPickup.generated.h"

UCLASS()
class GAINX_API AGainXHealthPickup : public AGainXBasePickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float HealthAmount = 100.0;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
