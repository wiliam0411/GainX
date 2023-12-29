// GainX, All Rights Reserved

#include "System/GainXAssetManager.h"
#include "GainXGameplayTags.h"

UGainXAssetManager::UGainXAssetManager() {}

UGainXAssetManager& UGainXAssetManager::Get()
{
    check(GEngine);

    UGainXAssetManager* GainXAssetManager = Cast<UGainXAssetManager>(GEngine->AssetManager);
    return *GainXAssetManager;
}