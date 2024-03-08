// GainX, All Rights Reserved

#include "Player/GainXPlayerState.h"
#include "Player/GainXPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/GainXAbilitySet.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXPlayerState, All, All);

AGainXPlayerState::AGainXPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    AbilitySystemComponent = CreateDefaultSubobject<UGainXAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

    NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* AGainXPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AGainXPlayerState::SetAbilitySet(const UGainXAbilitySet* InAbilitySet)
{
    check(InAbilitySet);

    if (InAbilitySet)
    {
        InAbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
    }
}

void AGainXPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    check(AbilitySystemComponent);
    AbilitySystemComponent->InitAbilityActorInfo(this, GetOwner());
}
