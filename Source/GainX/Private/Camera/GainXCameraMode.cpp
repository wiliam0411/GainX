// GainX, All Rights Reserved

#include "Camera/GainXCameraMode.h"
#include "Camera/GainXCameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/GainXPlayerCameraManager.h"

UGainXCameraMode::UGainXCameraMode() 
{
    FieldOfView = GAINX_CAMERA_DEFAULT_FOV;
    ViewPitchMin = GAINX_CAMERA_DEFAULT_PITCH_MIN;
    ViewPitchMax = GAINX_CAMERA_DEFAULT_PITCH_MAX;
}

UGainXCameraComponent* UGainXCameraMode::GetGainXCameraComponent() const
{
    return CastChecked<UGainXCameraComponent>(GetOuter());
}

AActor* UGainXCameraMode::GetTargetActor() const
{
    const UGainXCameraComponent* GainXCameraComponent = GetGainXCameraComponent();

    return GainXCameraComponent->GetTargetActor();
}

FVector UGainXCameraMode::GetPivotLocation() const
{
    const AActor* TargetActor = GetTargetActor();
    check(TargetActor);

    if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
    {
        // Height adjustments for characters to account for crouching.
        if (const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn))
        {
            const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
            check(TargetCharacterCDO);

            const UCapsuleComponent* CapsuleComp = TargetCharacter->GetCapsuleComponent();
            check(CapsuleComp);

            const UCapsuleComponent* CapsuleCompCDO = TargetCharacterCDO->GetCapsuleComponent();
            check(CapsuleCompCDO);

            const float DefaultHalfHeight = CapsuleCompCDO->GetUnscaledCapsuleHalfHeight();
            const float ActualHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
            const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight;

            return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
        }

        return TargetPawn->GetPawnViewLocation();
    }

    return TargetActor->GetActorLocation();
}

FRotator UGainXCameraMode::GetPivotRotation() const
{
    const AActor* TargetActor = GetTargetActor();
    check(TargetActor);

    if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
    {
        return TargetPawn->GetViewRotation();
    }

    return TargetActor->GetActorRotation();
}

void UGainXCameraMode::UpdateView(float DeltaTime) 
{
    FVector PivotLocation = GetPivotLocation();
    FRotator PivotRotation = GetPivotRotation();

    PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

    View.Location = PivotLocation;
    View.Rotation = PivotRotation;
    View.ControlRotation = View.Rotation;
    View.FieldOfView = FieldOfView;
}
