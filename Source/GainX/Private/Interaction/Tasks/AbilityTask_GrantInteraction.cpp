// GainX, All Rights Reserved

#include "Interaction/Tasks/AbilityTask_GrantInteraction.h"
#include "Interaction/InteractionQuery.h"
#include "Interaction/InteractionOption.h"
#include "AbilitySystemComponent.h"

UAbilityTask_GrantInteraction::UAbilityTask_GrantInteraction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UAbilityTask_GrantInteraction::Activate()
{
    Super::Activate();

    SetWaitingOnAvatar();

    GetWorld()->GetTimerManager().SetTimer(QueryTimerHandle, this, &ThisClass::QueryInteractables, InteractionScanRate, true);
}

void UAbilityTask_GrantInteraction::OnDestroy(bool AbilityEnded)
{
    Super::OnDestroy(AbilityEnded);
}

UAbilityTask_GrantInteraction* UAbilityTask_GrantInteraction::GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate)
{
    UAbilityTask_GrantInteraction* MyObj = NewAbilityTask<UAbilityTask_GrantInteraction>(OwningAbility);
    MyObj->InteractionScanRange = InteractionScanRange;
    MyObj->InteractionScanRate = InteractionScanRate;
    return MyObj;
}

void UAbilityTask_GrantInteraction::QueryInteractables()
{
    if (!GetWorld() || !GetAvatarActor())
    {
        return;
    }

    // Do sphere trace in order to get near interactable actors
    FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_GrantInteraction), false);
    TArray<FOverlapResult> OverlapResults;
    GetWorld()->OverlapMultiByChannel(OUT OverlapResults, GetAvatarActor()->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(InteractionScanRange), Params);

    TArray<TScriptInterface<IInteractableTarget>> NewInteractableTargets;
    for (const FOverlapResult& Overlap : OverlapResults)
    {
        TScriptInterface<IInteractableTarget> InteractableActor(Overlap.GetActor());
        if (InteractableActor)
        {
            NewInteractableTargets.AddUnique(InteractableActor);
        }
    }

    // Remove ability
    for (auto It = InteractableTargets.CreateIterator(); It; ++It)
    {
        if (!NewInteractableTargets.Contains(It.Key()))
        {
            AbilitySystemComponent->ClearAbility(It.Value());
            It.RemoveCurrent();
        }
    }

    for (TScriptInterface<IInteractableTarget>& NewInteractableTarget : NewInteractableTargets)
    {
        if (!InteractableTargets.Contains(NewInteractableTarget))
        {
            FGameplayAbilitySpec Spec(NewInteractableTarget->GetInteractionAbility(), 1, INDEX_NONE, this);
            FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
            InteractableTargets.Add(NewInteractableTarget, Handle);
        }
    }
}
