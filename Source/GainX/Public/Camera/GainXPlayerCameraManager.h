// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GainXPlayerCameraManager.generated.h"

#define GAINX_CAMERA_DEFAULT_FOV (80.0f)
#define GAINX_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define GAINX_CAMERA_DEFAULT_PITCH_MAX (89.0f)

/**
 * The base player camera manager class used by this project
 */
UCLASS()
class GAINX_API AGainXPlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    AGainXPlayerCameraManager(const FObjectInitializer& ObjectInitializer);
};
