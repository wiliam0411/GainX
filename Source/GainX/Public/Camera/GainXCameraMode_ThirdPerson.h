// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/GainXCameraMode.h"
#include "GainXCameraMode_ThirdPerson.generated.h"

class UCurveVector;

/**
 * The basic third person camera mode
 */
UCLASS(Blueprintable)
class GAINX_API UGainXCameraMode_ThirdPerson : public UGainXCameraMode
{
    GENERATED_BODY()

public:
    UGainXCameraMode_ThirdPerson();

protected:
    virtual void UpdateView(float DeltaTime) override;

    void UpdateForTarget(float DeltaTime);

    void SetTargetCrouchOffset(FVector NewTargetOffset);
    void UpdateCrouchOffset(float DeltaTime);

	virtual void DrawDebug(UCanvas* Canvas) const override;

protected:
    /* Once that is resolved this will become the default and TargetOffsetCurve will be removed */
    UPROPERTY(EditDefaultsOnly, Category = "Third Person")
    bool bUseRuntimeFloatCurves;

    /* Curve that defines local-space offsets from the target using the view pitch to evaluate the curve */
    UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "!bUseRuntimeFloatCurves"))
    TObjectPtr<const UCurveVector> TargetOffsetCurve;

    UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
    FRuntimeFloatCurve TargetOffsetX;

    UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
    FRuntimeFloatCurve TargetOffsetY;

    UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
    FRuntimeFloatCurve TargetOffsetZ;

    /* Alters the speed that a crouch offset is blended in or out */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Third Person")
    float CrouchOffsetBlendMultiplier = 5.0f;

    FVector InitialCrouchOffset = FVector::ZeroVector;
    FVector TargetCrouchOffset = FVector::ZeroVector;
    float CrouchOffsetBlendPct = 1.0f;
    FVector CurrentCrouchOffset = FVector::ZeroVector;
};
