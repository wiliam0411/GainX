// GainX, All Rights Reserved

#include "AbilitySystem/GainXGameplayAbility.h"
//#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "Player/GainXBaseCharacter.h"

UGainXGameplayAbility::UGainXGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    ActivationPolicy = EGainXAbilityActivationPolicy::OnInputTriggered;
}
UGainXAbilitySystemComponent* UGainXGameplayAbility::GetGainXAbilitySystemComponentFromActorInfo() const
{
    if (!CurrentActorInfo) return nullptr;

    return Cast<UGainXAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
}

AGainXBaseCharacter* UGainXGameplayAbility::GetGainXCharacterFromActorInfo() const
{
    if (!CurrentActorInfo) return nullptr;
    return CastChecked<AGainXBaseCharacter>(CurrentActorInfo->AvatarActor.Get());
}