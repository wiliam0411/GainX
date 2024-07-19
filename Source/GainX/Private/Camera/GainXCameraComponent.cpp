// GainX, All Rights Reserved

#include "Camera/GainXCameraComponent.h"
#include "Camera/GainXCameraMode.h"

UGainXCameraComponent::UGainXCameraComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UGainXCameraComponent* UGainXCameraComponent::FindCameraComponent(const AActor* Actor)
{
    return Actor ? Actor->FindComponentByClass<UGainXCameraComponent>() : nullptr;
}

void UGainXCameraComponent::OnRegister()
{
    Super::OnRegister();
}

void UGainXCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) {}

void UGainXCameraComponent::UpdateCameraModes()
{
    if (DetermineCameraModeDelegate.IsBound())
    {
        const TSubclassOf<UGainXCameraMode> CameraMode = DetermineCameraModeDelegate.Execute();
    }
}
