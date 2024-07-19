// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GainXCameraMode.generated.h"

class UGainXCameraComponent;

/**
 * View data produced by the camera mode that is used to blend camera modes.
 */
struct FGainXCameraModeView
{
public:
    FGainXCameraModeView() : Location(ForceInit), Rotation(ForceInit), ControlRotation(ForceInit), FieldOfView(80) {}

public:
    FVector Location;

    FRotator Rotation;

    FRotator ControlRotation;

    float FieldOfView;
};

/**
 * Base class for all camera modes
 */
UCLASS(Abstract, NotBlueprintable)
class GAINX_API UGainXCameraMode : public UObject
{
    GENERATED_BODY()

public:
    UGainXCameraMode();

    UGainXCameraComponent* GetGainXCameraComponent() const;

    AActor* GetTargetActor() const;

protected:  
    virtual FVector GetPivotLocation() const;

    virtual FRotator GetPivotRotation() const;

    virtual void UpdateView(float DeltaTime);

protected:
    /* View output produced by the camera mode */
    FGainXCameraModeView View;

    /* The horizontal field of view(in degrees) */
    UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
    float FieldOfView;

    /* Minimum view pitch(in degrees) */
    UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
    float ViewPitchMin;

    /* Maximum view pitch (in degrees) */
    UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
    float ViewPitchMax;
};
