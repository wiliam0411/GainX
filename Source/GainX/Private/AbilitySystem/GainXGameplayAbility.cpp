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

void UGainXGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    K2_OnAbilityAdded();

    TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

bool UGainXGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) || !ActorInfo)
    {
        return false;
    }

    for (TObjectPtr<UGainXAbilityCost> AdditionalCost : AdditionalCosts)
    {
        if (AdditionalCost && !AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
        {
            return false;
        }
    }

    return true;
}

void UGainXGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

    for (TObjectPtr<UGainXAbilityCost> AdditionalCost : AdditionalCosts)
    {
        if (AdditionalCost)
        {
            AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
        }
    }
}