// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GainXCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAINX_API UGainXCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "10.0"))
    float RunMultiplier = 2.0f;

    virtual float GetMaxSpeed() const override;
};
