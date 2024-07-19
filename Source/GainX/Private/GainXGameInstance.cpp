// GainX, All Rights Reserved

#include "GainXGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"

UGainXGameInstance::UGainXGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXGameInstance::Init() 
{
    Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
}

void UGainXGameInstance::Shutdown() 
{
    Super::Shutdown();
}
