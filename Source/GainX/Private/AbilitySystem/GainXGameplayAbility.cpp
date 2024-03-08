// GainX, All Rights Reserved

#include "AbilitySystem/GainXGameplayAbility.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "Player/GainXPlayerController.h"
#include "Player/GainXPlayerCharacter.h"

UGainXGameplayAbility::UGainXGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    ActivationPolicy = EGainXAbilityActivationPolicy::OnInputTriggered;
}
UGainXAbilitySystemComponent* UGainXGameplayAbility::GetGainXAbilitySystemComponentFromActorInfo() const
{
    if (!CurrentActorInfo) return nullptr;
    return Cast<UGainXAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
}

AGainXPlayerController* UGainXGameplayAbility::GetGainXPlayerControllerFromActorInfo() const
{
    if (!CurrentActorInfo) return nullptr;
    return Cast<AGainXPlayerController>(CurrentActorInfo->PlayerController.Get());
}

AGainXPlayerCharacter* UGainXGameplayAbility::GetGainXCharacterFromActorInfo() const
{
    if (!CurrentActorInfo) return nullptr;
    UE_LOG(LogTemp, Display, TEXT("AvatarActor is %d"), CurrentActorInfo->AvatarActor.IsValid());
    return Cast<AGainXPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
}

void UGainXGameplayAbility::OnPawnAvatarSet() 
{
    K2_OnPawnAvatarSet();
}
