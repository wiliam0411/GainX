// GainX, All Rights Reserved

#include "GameFeatures/GameFeatureAction_AddWidgets.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/GameInstance.h"
#include "GameFeaturesSubsystemSettings.h"
#include "CommonUIExtensions.h"
#include "UI/GainXHUD.h"


#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "CommonLocalPlayer.h"

void UGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    Super::OnGameFeatureDeactivating(Context);

    FPerContextData* ActiveData = ContextData.Find(Context);
    if ensure (ActiveData)
    {
        Reset(*ActiveData);
    }
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddWidgets::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
    for (const FGainXHUDElementEntry& Entry : Widgets)
    {
        AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Entry.WidgetClass.ToSoftObjectPath().GetAssetPath());
    }
}
#endif

void UGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
    UWorld* World = WorldContext.World();
    UGameInstance* GameInstance = WorldContext.OwningGameInstance;
    FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

    if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
    {
        if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
        {
            TSoftClassPtr<AActor> HUDActorClass = AGainXHUD::StaticClass();

            TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
                ComponentManager->AddExtensionHandler(HUDActorClass, UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
            ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
        }
    }
}

void UGameFeatureAction_AddWidgets::Reset(FPerContextData& ActiveData)
{
    ActiveData.ComponentRequests.Empty();

    for (TPair<FObjectKey, FPerActorData>& Pair : ActiveData.ActorData)
    {
        for (FUIExtensionHandle& Handle : Pair.Value.ExtensionHandles)
        {
            Handle.Unregister();
        }
    }
    ActiveData.ActorData.Empty();
}

void UGameFeatureAction_AddWidgets::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
    FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
    if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
    {
        RemoveWidgets(Actor, ActiveData);
    }
    else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
    {
        AddWidgets(Actor, ActiveData);
    }
}

void UGameFeatureAction_AddWidgets::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
    AGainXHUD* HUD = CastChecked<AGainXHUD>(Actor);

    if (!HUD->GetOwningPlayerController())
    {
        return;
    }

    if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
    {
        FPerActorData& ActorData = ActiveData.ActorData.FindOrAdd(HUD);

        for (const FGainXHUDLayoutRequest& Entry : Layout)
        {
            if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
            {
                ActorData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
            }
        }

        UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
        for (const FGainXHUDElementEntry& Entry : Widgets)
        {
            ActorData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
        }
    }
}

void UGameFeatureAction_AddWidgets::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
    AGainXHUD* HUD = CastChecked<AGainXHUD>(Actor);

    // Only unregister if this is the same HUD actor that was registered, there can be multiple active at once on the client
    FPerActorData* ActorData = ActiveData.ActorData.Find(HUD);

    if (ActorData)
    {
        for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActorData->LayoutsAdded)
        {
            if (AddedLayout.IsValid())
            {
                AddedLayout->DeactivateWidget();
            }
        }

        for (FUIExtensionHandle& Handle : ActorData->ExtensionHandles)
        {
            Handle.Unregister();
        }
        ActiveData.ActorData.Remove(HUD);
    }
}
