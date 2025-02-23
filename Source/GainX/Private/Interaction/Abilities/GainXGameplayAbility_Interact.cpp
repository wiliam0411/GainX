// GainX, All Rights Reserved


#include "Interaction/Abilities/GainXGameplayAbility_Interact.h"
#include "AbilitySystemComponent.h"
#include "Interaction/Tasks/AbilityTask_GrantInteraction.h"


UGainXGameplayAbility_Interact::UGainXGameplayAbility_Interact() {}

void UGainXGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) 
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
    if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
    {
        UAbilityTask_GrantInteraction* Task = UAbilityTask_GrantInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
        Task->ReadyForActivation();
    }
}