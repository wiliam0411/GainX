// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GainXCharacterMovementComponent.generated.h"

UCLASS()
class GAINX_API UGainXCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    /* Calculates distance from character to ground for AnimBP */
    UFUNCTION(BlueprintCallable, Category = "GainX|CharacterMovement")
    float GetGroundDistance();

protected:
    /* Defines length of trace to calculate GroundDistance */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float GroundTraceDistance = 600.0f;
};
