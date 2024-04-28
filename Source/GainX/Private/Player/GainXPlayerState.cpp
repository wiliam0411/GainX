// GainX, All Rights Reserved

#include "Player/GainXPlayerState.h"
#include "Player/GainXPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/GainXAbilitySet.h"
#include "AbilitySystem/Attributes/GainXHealthSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXPlayerState, All, All);

AGainXPlayerState::AGainXPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NetUpdateFrequency = 100.0f;
}