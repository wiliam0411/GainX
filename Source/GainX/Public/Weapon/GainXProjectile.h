// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GainXProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UGainXWeaponFXComponent;

UCLASS()
class GAINX_API AGainXProjectile : public AActor
{
    GENERATED_BODY()

public:
    AGainXProjectile();
};
