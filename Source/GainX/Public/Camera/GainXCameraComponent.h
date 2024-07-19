// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GainXCameraComponent.generated.h"

class UGainXCameraMode;

DECLARE_DELEGATE_RetVal(TSubclassOf<UGainXCameraMode>, FGainXCameraModeDelegate);

/**
 * The base camera component class used by this project
 */
UCLASS()
class GAINX_API UGainXCameraComponent : public UCameraComponent
{
    GENERATED_BODY()

public:
    UGainXCameraComponent(const FObjectInitializer& ObjectInitializer);

    /* Delegate used to query for the best camera mode */
    FGainXCameraModeDelegate DetermineCameraModeDelegate;

    /* Returns the camera component if one exists on the specified actor */
    UFUNCTION(BlueprintPure, Category = "GainX|Camera")
    static UGainXCameraComponent* FindCameraComponent(const AActor* Actor);

    /* Returns the target actor that the camera is looking at */
    virtual AActor* GetTargetActor() const { return GetOwner(); }

protected:
    virtual void OnRegister() override;
    virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

    virtual void UpdateCameraModes();
};
