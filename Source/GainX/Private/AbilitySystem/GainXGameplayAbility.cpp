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
    if (!CurrentActorInfo)
    {
        return nullptr;
    }
    return Cast<AGainXPlayerController>(CurrentActorInfo->PlayerController.Get());
}

AGainXPlayerCharacter* UGainXGameplayAbility::GetGainXCharacterFromActorInfo() const
{
    if (!CurrentActorInfo)
    {
        return nullptr;
    }
    return Cast<AGainXPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
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