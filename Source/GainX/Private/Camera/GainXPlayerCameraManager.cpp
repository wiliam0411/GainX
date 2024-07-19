// GainX, All Rights Reserved

#include "Camera/GainXPlayerCameraManager.h"

AGainXPlayerCameraManager::AGainXPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    DefaultFOV = GAINX_CAMERA_DEFAULT_FOV;
    ViewPitchMin = GAINX_CAMERA_DEFAULT_PITCH_MIN;
    ViewPitchMax = GAINX_CAMERA_DEFAULT_PITCH_MAX;
}
