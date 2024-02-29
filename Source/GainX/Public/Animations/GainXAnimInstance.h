// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GainXAnimInstance.generated.h"

UCLASS()
class GAINX_API UGainXAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UGainXAnimInstance(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterStateData")
    float GroundDistance = -1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "WeaponSocket")
    FName LeftHandSocketName = "LeftHand";

    UPROPERTY(BlueprintReadOnly, Category = "WeaponSocket")
    FTransform LeftHandSocketTransform = FTransform::Identity;

private:
    FName RightHandBoneName = "hand_r";
};
