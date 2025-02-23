// GainX, All Rights Reserved

#include "AbilitySystem/GainXGameplayAbility.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "Player/GainXPlayerController.h"
#include "Player/GainXPlayerCharacter.h"
#include "AbilitySystem/Abilities/GainXAbilityCost.h"

UGainXGameplayAbility::UGainXGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

    ActivationPolicy = EGainXAbilityActivationPolicy::OnInputTriggered;
}
UGainXAbilitySystemComponent* UGainXGameplayAbility::GetGainXAbilitySystemComponentFromActorInfo() const
{
    if (!CurrentActorInfo)
    {
        return nullptr;
    }
    return Cast<UGainXAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
}

AGainXPlayerController* UGainXGameplayAbility::GetGainXPlayerControllerFromActorInfo() const
{
    if (CurrentActorInfo)
    {
        if (const auto GainXCharacter = Cast<AGainXBaseCharacter>(CurrentActorInfo->OwnerActor.Get()))
        {
            return GainXCharacter->GetGainXPlayerController();
        }
    }

    return nullptr;
}

AGainXPlayerCharacter* UGainXGameplayAbility::GetGainXCharacterFromActorInfo() const
{
    if (!CurrentActorInfo)
    {
        return nullptr;
    }
    return Cast<AGainXPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
}

void UGainXGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
    // Try to activate if activation policy is on spawn.
    if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EGainXAbilityActivationPolicy::OnSpawn))
    {
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
        const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

        // If avatar actor is torn off or about to die, don't try to activate until we get the new one.
        if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
        {
            ASC->TryActivateAbility(Spec.Handle);
        }
    }
}

void UGainXGameplayAbility::OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) 
{
    K2_OnAbilityFailedToActivate(FailedReason);
}

void UGainXGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    K2_OnAbilityAdded();

    TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UGainXGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) 
{
    K2_OnAbilityRemoved();

    Super::OnRemoveAbility(ActorInfo, Spec);
}