// GainX, All Rights Reserved

#include "System/GainXAssetManager.h"
#include "GainXGameplayTags.h"

UGainXAssetManager::UGainXAssetManager() {}

UGainXAssetManager& UGainXAssetManager::Get()
{
    check(GEngine);

    if (UGainXAssetManager* Singleton = Cast<UGainXAssetManager>(GEngine->AssetManager))
    {
        return *Singleton;
    }

    UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to GainXAssetManager!"));

    // Fatal error above prevents this from being called.
    return *NewObject<UGainXAssetManager>();
}

UObject* UGainXAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
    if (AssetPath.IsValid())
    {
        TUniquePtr<FScopeLogTime> LogTimePtr;

        if (ShouldLogAssetLoads())
        {
            LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
        }

        if (UAssetManager::IsInitialized())
        {
            return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
        }

        // Use LoadObject if asset manager isn't ready yet.
        return AssetPath.TryLoad();
    }

    return nullptr;
}

bool UGainXAssetManager::ShouldLogAssetLoads()
{
    static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
    return bLogAssetLoads;
}

void UGainXAssetManager::AddLoadedAsset(const UObject* Asset)
{
    if (ensureAlways(Asset))
    {
        FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
        LoadedAssets.Add(Asset);
    }
}
