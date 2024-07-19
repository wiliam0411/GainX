// GainX, All Rights Reserved

#include "GameModes/GainXWorldSettings.h"
#include "Engine/AssetManager.h"

AGainXWorldSettings::AGainXWorldSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

FPrimaryAssetId AGainXWorldSettings::GetDefaultGameplayExperience() const
{
    FPrimaryAssetId Result;
    if (!DefaultGameplayExperience.IsNull())
    {
        Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

        if (!Result.IsValid())
        {
            UE_LOG(LogTemp, Error,
                TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
                *GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
        }
    }
    return Result;
}
