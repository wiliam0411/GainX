// GainX, All Rights Reserved

#include "GameModes/GainXGameState.h"
#include "GameModes/GainXExperienceManagerComponent.h"

AGainXGameState::AGainXGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

	ExperienceManagerComponent = CreateDefaultSubobject<UGainXExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
