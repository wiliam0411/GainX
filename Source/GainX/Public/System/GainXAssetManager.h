// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GainXAssetManager.generated.h"

/**
 * UGainXAssetManager
 *
 * Game implementation of the asset manager that overrides functionality and stores game-specific types.
 * It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 * This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 */
UCLASS()
class GAINX_API UGainXAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    UGainXAssetManager();

    static UGainXAssetManager& Get();

    // Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
    template <typename AssetType>
    static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

protected:
    static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
    static bool ShouldLogAssetLoads();

    // Thread safe way of adding a loaded asset to keep in memory.
    void AddLoadedAsset(const UObject* Asset);

private:
    // Assets loaded and tracked by the asset manager.
    UPROPERTY()
    TSet<TObjectPtr<const UObject>> LoadedAssets;

    // Used for a scope lock when modifying the list of load assets.
    FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UGainXAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
    AssetType* LoadedAsset = nullptr;

    const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

    if (AssetPath.IsValid())
    {
        LoadedAsset = AssetPointer.Get();
        if (!LoadedAsset)
        {
            LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
            ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
        }

        if (LoadedAsset && bKeepInMemory)
        {
            // Added to loaded asset list.
            Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
        }
    }

    return LoadedAsset;
}
