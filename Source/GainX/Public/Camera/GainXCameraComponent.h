// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GainXCameraComponent.generated.h"

class UGainXCameraMode;
class UGainXCameraModeStack;
struct FGameplayTag;

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

    // Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
    void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

    /* Returns the camera component if one exists on the specified actor */
    UFUNCTION(BlueprintPure, Category = "GainX|Camera")
    static UGainXCameraComponent* FindCameraComponent(const AActor* Actor);

    /* Returns the target actor that the camera is looking at */
    virtual AActor* GetTargetActor() const { return GetOwner(); }

    // Gets the tag associated with the top layer and the blend weight of it
    void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
    virtual void OnRegister() override;
    virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

    virtual void UpdateCameraModes();

protected:
    // Stack used to blend the camera modes.
    UPROPERTY()
    TObjectPtr<UGainXCameraModeStack> CameraModeStack;

    // Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
    float FieldOfViewOffset;
};
