// GainX, All Rights Reserved

#include "Camera/GainXCameraMode_ThirdPerson.h"
#include "GameFramework/Character.h"
#include "Curves/CurveVector.h"

UGainXCameraMode_ThirdPerson::UGainXCameraMode_ThirdPerson() 
{
    TargetOffsetCurve = nullptr;
}

void UGainXCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
    UpdateForTarget(DeltaTime);
    UpdateCrouchOffset(DeltaTime);

    FVector PivotLocation = GetPivotLocation() + CurrentCrouchOffset;
    FRotator PivotRotation = GetPivotRotation();

    PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

    View.Location = PivotLocation;
    View.Rotation = PivotRotation;
    View.ControlRotation = View.Rotation;
    View.FieldOfView = FieldOfView;

    // Apply third person offset using pitch.
    if (!bUseRuntimeFloatCurves)
    {
        if (TargetOffsetCurve)
        {
            const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
            View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
        }
    }
    else
    {
        FVector TargetOffset(0.0f);

        TargetOffset.X = TargetOffsetX.GetRichCurveConst()->Eval(PivotRotation.Pitch);
        TargetOffset.Y = TargetOffsetY.GetRichCurveConst()->Eval(PivotRotation.Pitch);
        TargetOffset.Z = TargetOffsetZ.GetRichCurveConst()->Eval(PivotRotation.Pitch);

        View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
    }
}

void UGainXCameraMode_ThirdPerson::UpdateForTarget(float DeltaTime)
{
    if (const ACharacter* TargetCharacter = Cast<ACharacter>(GetTargetActor()))
    {
        if (TargetCharacter->bIsCrouched)
        {
            const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
            const float CrouchedHeightAdjustment = TargetCharacterCDO->CrouchedEyeHeight - TargetCharacterCDO->BaseEyeHeight;

            SetTargetCrouchOffset(FVector(0.f, 0.f, CrouchedHeightAdjustment));

            return;
        }
    }

    SetTargetCrouchOffset(FVector::ZeroVector);
}

void UGainXCameraMode_ThirdPerson::SetTargetCrouchOffset(FVector NewTargetOffset)
{
    CrouchOffsetBlendPct = 0.0f;
    InitialCrouchOffset = CurrentCrouchOffset;
    TargetCrouchOffset = NewTargetOffset;
}

void UGainXCameraMode_ThirdPerson::UpdateCrouchOffset(float DeltaTime)
{
    if (CrouchOffsetBlendPct < 1.0f)
    {
        CrouchOffsetBlendPct = FMath::Min(CrouchOffsetBlendPct + DeltaTime * CrouchOffsetBlendMultiplier, 1.0f);
        CurrentCrouchOffset = FMath::InterpEaseInOut(InitialCrouchOffset, TargetCrouchOffset, CrouchOffsetBlendPct, 1.0f);
    }
    else
    {
        CurrentCrouchOffset = TargetCrouchOffset;
        CrouchOffsetBlendPct = 1.0f;
    }
}
