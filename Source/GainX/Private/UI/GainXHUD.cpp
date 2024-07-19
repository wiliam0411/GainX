// GainX, All Rights Reserved

#include "UI/GainXHUD.h"
#include "Components/GameFrameworkComponentManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXGameHUD, All, All);

AGainXHUD::AGainXHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    PrimaryActorTick.bStartWithTickEnabled = false;
}

void AGainXHUD::PreInitializeComponents() 
{
    Super::PreInitializeComponents();

    UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AGainXHUD::BeginPlay() 
{
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void AGainXHUD::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
    UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

    Super::EndPlay(EndPlayReason);
}
