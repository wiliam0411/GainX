// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GainXCoreTypes.h"
#include "GainXWeaponComponent.generated.h"

class AGainXBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAINX_API UGainXWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGainXWeaponComponent();
};