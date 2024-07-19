// GainX, All Rights Reserved

#include "GameModes/GainXExperienceManagerComponent.h"
#include "System/GainXAssetManager.h"
#include "GameModes/GainXExperience.h"
#include "GameModes/GainXExperienceActionSet.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameFeaturesSubsystem.h"

UGainXExperienceManagerComponent::UGainXExperienceManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXExperienceManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // deactivate any features this experience loaded
    //@TODO: This should be handled FILO as well
    for (const FString& PluginURL : GameFeaturePluginURLs)
    {
        UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
    }

    //@TODO: Ensure proper handling of a partially-loaded state too
    if (LoadState == EGainXExperienceLoadState::Loaded)
    {
        LoadState = EGainXExperienceLoadState::Deactivating;
    }
}

void UGainXExperienceManagerComponent::SetCurrentExperience(FPrimaryAssetId ExperienceId)
{
    // Loads experience class
    UGainXAssetManager& AssetManager = UGainXAssetManager::Get();
    FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
    TSubclassOf<UGainXExperience> AssetClass = Cast<UClass>(AssetPath.TryLoad());
    check(AssetClass);

    const auto Experience = GetDefault<UGainXExperience>(AssetClass);

    check(Experience != nullptr);
    check(CurrentExperience == nullptr);
    CurrentExperience = Experience;

    StartExperienceLoad();
}

const UGainXExperience* UGainXExperienceManagerComponent::GetCurrentExperienceChecked() const
{
    check(LoadState == EGainXExperienceLoadState::Loaded);
    check(CurrentExperience != nullptr);
    return CurrentExperience;
}

bool UGainXExperienceManagerComponent::IsExperienceLoaded() const
{
    return (LoadState == EGainXExperienceLoadState::Loaded) && (CurrentExperience != nullptr);
}

void UGainXExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnGainXExperienceLoaded::FDelegate&& Delegate) 
{
    if (IsExperienceLoaded())
    {
        Delegate.Execute(CurrentExperience);
    }
    else
    {
        OnExperienceLoaded.Add(MoveTemp(Delegate));
    }
}

void UGainXExperienceManagerComponent::StartExperienceLoad()
{
    check(CurrentExperience != nullptr);
    check(LoadState == EGainXExperienceLoadState::Unloaded);

    UE_LOG(LogTemp, Log, TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s)"), *CurrentExperience->GetPrimaryAssetId().ToString());

    LoadState = EGainXExperienceLoadState::Loading;

    UGainXAssetManager& AssetManager = UGainXAssetManager::Get();

    TSet<FPrimaryAssetId> BundleAssetList;
    TSet<FSoftObjectPath> RawAssetList;

    BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
    for (const TObjectPtr<UGainXExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
    {
        if (ActionSet != nullptr)
        {
            BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
        }
    }

    // Load assets associated with the experience

    TArray<FName> BundlesToLoad;

    const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
    const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
    const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
    if (bLoadClient)
    {
        BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
    }
    if (bLoadServer)
    {
        BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
    }

    TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
    if (BundleAssetList.Num() > 0)
    {
        BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
    }

    TSharedPtr<FStreamableHandle> RawLoadHandle = nullptr;
    if (RawAssetList.Num() > 0)
    {
        RawLoadHandle = AssetManager.LoadAssetList(RawAssetList.Array(), FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority, TEXT("StartExperienceLoad()"));
    }

    // If both async loads are running, combine them
    TSharedPtr<FStreamableHandle> Handle = nullptr;
    if (BundleLoadHandle.IsValid() && RawLoadHandle.IsValid())
    {
        Handle = AssetManager.GetStreamableManager().CreateCombinedHandle({BundleLoadHandle, RawLoadHandle});
    }
    else
    {
        Handle = BundleLoadHandle.IsValid() ? BundleLoadHandle : RawLoadHandle;
    }

    FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
    if (!Handle.IsValid() || Handle->HasLoadCompleted())
    {
        // Assets were already loaded, call the delegate now
        FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
    }
    else
    {
        Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

        Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]() { OnAssetsLoadedDelegate.ExecuteIfBound(); }));
    }

    // This set of assets gets preloaded, but we don't block the start of the experience based on it
    TSet<FPrimaryAssetId> PreloadAssetList;
    //@TODO: Determine assets to preload (but not blocking-ly)
    if (PreloadAssetList.Num() > 0)
    {
        AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
    }
}

void UGainXExperienceManagerComponent::OnExperienceLoadComplete()
{
    check(LoadState == EGainXExperienceLoadState::Loading);
    check(CurrentExperience != nullptr);

    UE_LOG(LogTemp, Log, TEXT("EXPERIENCE: OnExperienceLoadComplete(CurrentExperience = %s)"), *CurrentExperience->GetPrimaryAssetId().ToString());

    // Find the URLs for our GameFeaturePlugins - filtering out dupes and ones that don't have a valid mapping
    GameFeaturePluginURLs.Reset();

    auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
    {
        for (const FString& PluginName : FeaturePluginList)
        {
            FString PluginURL;
            if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, /*out*/ PluginURL))
            {
                This->GameFeaturePluginURLs.AddUnique(PluginURL);
            }
            else
            {
                ensureMsgf(false, TEXT("OnExperienceLoadComplete failed to find plugin URL from PluginName %s for experience %s - fix data, ignoring for this run"), *PluginName,
                    *Context->GetPrimaryAssetId().ToString());
            }
        }
    };

    CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);
    for (const TObjectPtr<UGainXExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
    {
        if (ActionSet != nullptr)
        {
            CollectGameFeaturePluginURLs(ActionSet, ActionSet->GameFeaturesToEnable);
        }
    }

    // Load and activate the features
    NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
    if (NumGameFeaturePluginsLoading > 0)
    {
        LoadState = EGainXExperienceLoadState::LoadingGameFeatures;
        for (const FString& PluginURL : GameFeaturePluginURLs)
        {
            // UGainXExperienceManager::NotifyOfPluginActivation(PluginURL);
            UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
        }
    }
    else
    {
        OnExperienceFullLoadCompleted();
    }
}

void UGainXExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
    // decrement the number of plugins that are loading
    NumGameFeaturePluginsLoading--;

    if (NumGameFeaturePluginsLoading == 0)
    {
        OnExperienceFullLoadCompleted();
    }
}

void UGainXExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
    UE_LOG(LogTemp, Log, TEXT("UGainXExperienceManagerComponent::OnExperienceFullLoadCompleted"));

    check(LoadState != EGainXExperienceLoadState::Loaded);

    LoadState = EGainXExperienceLoadState::ExecutingActions;

    // Execute the actions
    FGameFeatureActivatingContext Context;

    // Only apply to our specific world context if set
    const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
    if (ExistingWorldContext)
    {
        Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
    }

    auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
    {
        for (UGameFeatureAction* Action : ActionList)
        {
            if (Action != nullptr)
            {
                //@TODO: The fact that these don't take a world are potentially problematic in client-server PIE
                // The current behavior matches systems like gameplay tags where loading and registering apply to the entire process,
                // but actually applying the results to actors is restricted to a specific world
                Action->OnGameFeatureRegistering();
                Action->OnGameFeatureLoading();
                Action->OnGameFeatureActivating(Context);
            }
        }
    };

    ActivateListOfActions(CurrentExperience->Actions);
    for (const TObjectPtr<UGainXExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
    {
        if (ActionSet != nullptr)
        {
            ActivateListOfActions(ActionSet->Actions);
        }
    }

    LoadState = EGainXExperienceLoadState::Loaded;

    OnExperienceLoaded.Broadcast(CurrentExperience);
    OnExperienceLoaded.Clear();
}