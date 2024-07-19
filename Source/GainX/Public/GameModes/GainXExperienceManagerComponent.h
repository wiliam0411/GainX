// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GainXExperienceManagerComponent.generated.h"

class UGainXExperience;

namespace UE::GameFeatures
{
struct FResult;
}

enum class EGainXExperienceLoadState
{
    Unloaded,
    Loading,
    LoadingGameFeatures,
    LoadingChaosTestingDelay,
    ExecutingActions,
    Loaded,
    Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGainXExperienceLoaded, const UGainXExperience*);

/**
 *
 */
UCLASS()
class GAINX_API UGainXExperienceManagerComponent : public UGameStateComponent
{
    GENERATED_BODY()

public:
    UGainXExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~UActorComponent interface
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //~End of UActorComponent interface

    /* Tries to set the current experience, either a UI or gameplay one */
    void SetCurrentExperience(FPrimaryAssetId ExperienceId);

    /* This returns the current experience if it is fully loaded, asserting otherwise */
    const UGainXExperience* GetCurrentExperienceChecked() const;

    /* Returns true if the experience is fully loaded */
    bool IsExperienceLoaded() const;

    /* Ensures the delegate is called once the experience has been loaded. If the experience has already loaded, calls the delegate immediately */
    void CallOrRegister_OnExperienceLoaded(FOnGainXExperienceLoaded::FDelegate&& Delegate);

private:
    void StartExperienceLoad();
    void OnExperienceLoadComplete();
    void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
    void OnExperienceFullLoadCompleted();

private:
    UPROPERTY()
    TObjectPtr<const UGainXExperience> CurrentExperience;

    EGainXExperienceLoadState LoadState = EGainXExperienceLoadState::Unloaded;

    TArray<FString> GameFeaturePluginURLs;

    int32 NumGameFeaturePluginsLoading = 0;

    FOnGainXExperienceLoaded OnExperienceLoaded;
};