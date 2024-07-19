// GainX, All Rights Reserved

#pragma once

#include "CommonActivatableWidget.h"
#include "GameFeatureAction_WorldAction.h"
#include "UIExtensionSystem.h"

#include "GameFeatureAction_AddWidgets.generated.h"

struct FWorldContext;
struct FComponentRequestHandle;

USTRUCT()
struct FGainXHUDLayoutRequest
{
    GENERATED_BODY()

public:
    /* The layout widget to spawn */
    UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
    TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

    /* The layer to insert the widget in */
    UPROPERTY(EditAnywhere, Category = UI, meta = (Categories = "UI.Layer"))
    FGameplayTag LayerID;
};

USTRUCT()
struct FGainXHUDElementEntry
{
    GENERATED_BODY()

public:
    /* The widget to spawn */
    UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
    TSoftClassPtr<UUserWidget> WidgetClass;

    /* The slot ID where we should place this widget */
    UPROPERTY(EditAnywhere, Category = UI)
    FGameplayTag SlotID;
};

/**
 *
 */
UCLASS(meta = (DisplayName = "Add Widgets"))
class GAINX_API UGameFeatureAction_AddWidgets final : public UGameFeatureAction_WorldAction
{
    GENERATED_BODY()

public:
    //~ Begin UGameFeatureAction interface
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#if WITH_EDITORONLY_DATA
    virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
    //~ End UGameFeatureAction interface

private:
    struct FPerActorData
    {
        TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
        TArray<FUIExtensionHandle> ExtensionHandles;
    };

    struct FPerContextData
    {
        TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
        TMap<FObjectKey, FPerActorData> ActorData;
    };

private:
    //~ Begin UGameFeatureAction_WorldAction interface
    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
    //~ End UGameFeatureAction_WorldAction interface

    void Reset(FPerContextData& ActiveData);

    void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

    void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
    void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

private:
    /* Layout to add to the HUD */
    UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{LayerID} -> {LayoutClass}"))
    TArray<FGainXHUDLayoutRequest> Layout;

    /* Widgets to add to the HUD */
    UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{SlotID} -> {WidgetClass}"))
    TArray<FGainXHUDElementEntry> Widgets;

    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
};
