// GainX, All Rights Reserved

#include "Interaction/Tasks/ScanInteractablesNearby.h"
#include "TimerManager.h"

UScanInteractablesNearby::UScanInteractablesNearby(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UScanInteractablesNearby* UScanInteractablesNearby::ScanInteractablesNearby(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate, ECollisionChannel CollisionChannel)
{
    UScanInteractablesNearby* NewTask = NewAbilityTask<UScanInteractablesNearby>(OwningAbility);
    NewTask->ScanRange = InteractionScanRange;
    NewTask->ScanRate = InteractionScanRate;
    NewTask->CollisionChannel = CollisionChannel;

    return NewTask;
}

void UScanInteractablesNearby::Activate()
{
    Super::Activate();

    SetWaitingOnAvatar();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(ScanTimerHandle, this, &ThisClass::PerformScan, ScanRate, true);
    }
}

void UScanInteractablesNearby::OnDestroy(bool AbilityEnded) 
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ScanTimerHandle);
    }

    Super::OnDestroy(AbilityEnded);
}

void UScanInteractablesNearby::PerformScan() 
{
    if (!GetWorld() || !GetAvatarActor())
    {
        return;
    }

    // Do sphere trace in order to get nearby interactable actors
    FCollisionQueryParams Params(SCENE_QUERY_STAT(UScanInteractablesNearby), false);
    TArray<FOverlapResult> OverlapResults;
    GetWorld()->OverlapMultiByChannel(OUT OverlapResults, GetAvatarActor()->GetActorLocation(), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(ScanRange), Params);

    TArray<AActor*> OverlapActors;
    for (const FOverlapResult& OverlapResult : OverlapResults)
    {
        if (AActor* OverlapActor = OverlapResult.GetActor())
        {
            OverlapActors.Add(OverlapActor);
        }
    }

    OnTaskUpdated.Broadcast(OverlapActors);
}