// GainX, All Rights Reserved

#include "Camera/GainXCameraComponent.h"
#include "Camera/GainXCameraMode.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UGainXCameraComponent::UGainXCameraComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CameraModeStack = nullptr;
    FieldOfViewOffset = 0.0f;
}

UGainXCameraComponent* UGainXCameraComponent::FindCameraComponent(const AActor* Actor)
{
    return Actor ? Actor->FindComponentByClass<UGainXCameraComponent>() : nullptr;
}

void UGainXCameraComponent::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
    check(CameraModeStack);
    CameraModeStack->GetBlendInfo(/*out*/ OutWeightOfTopLayer, /*out*/ OutTagOfTopLayer);
}

void UGainXCameraComponent::OnRegister()
{
    Super::OnRegister();

    if (!CameraModeStack)
    {
        CameraModeStack = NewObject<UGainXCameraModeStack>(this);
        check(CameraModeStack);
    }
}

void UGainXCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
    check(CameraModeStack);

    UpdateCameraModes();

    FGainXCameraModeView CameraModeView;
    CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

    // Keep player controller in sync with the latest view.
    if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
    {
        if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
        {
            PC->SetControlRotation(CameraModeView.ControlRotation);
        }
    }

    // Apply any offset that was added to the field of view.
    CameraModeView.FieldOfView += FieldOfViewOffset;
    FieldOfViewOffset = 0.0f;

    // Keep camera component in sync with the latest view.
    SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
    FieldOfView = CameraModeView.FieldOfView;

    // Fill in desired view.
    DesiredView.Location = CameraModeView.Location;
    DesiredView.Rotation = CameraModeView.Rotation;
    DesiredView.FOV = CameraModeView.FieldOfView;
    DesiredView.OrthoWidth = OrthoWidth;
    DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
    DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
    DesiredView.AspectRatio = AspectRatio;
    DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
    DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
    DesiredView.ProjectionMode = ProjectionMode;

    // See if the CameraActor wants to override the PostProcess settings used.
    DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
    if (PostProcessBlendWeight > 0.0f)
    {
        DesiredView.PostProcessSettings = PostProcessSettings;
    }

    if (IsXRHeadTrackedCamera())
    {
        // In XR much of the camera behavior above is irrellevant, but the post process settings are not.
        Super::GetCameraView(DeltaTime, DesiredView);
    }
}

void UGainXCameraComponent::UpdateCameraModes()
{
    check(CameraModeStack);

    if (CameraModeStack->IsStackActivate())
    {
        if (DetermineCameraModeDelegate.IsBound())
        {
            if (const TSubclassOf<UGainXCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
            {
                CameraModeStack->PushCameraMode(CameraMode);
            }
        }
    }
}
