// GainX, All Rights Reserved

#include "Interaction/Tasks/AbilityTask_LookForInteractables.h"
#include "Interaction/InteractableTarget.h"

UAbilityTask_LookForInteractables::UAbilityTask_LookForInteractables(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UAbilityTask_LookForInteractables* UAbilityTask_LookForInteractables::LookForInteractables(UGameplayAbility* OwningAbility, ECollisionChannel InteractionCollisionChannel, float InteractionScanRange, float InteractionScanRate)
{
    UAbilityTask_LookForInteractables* MyObj = NewAbilityTask<UAbilityTask_LookForInteractables>(OwningAbility);
    MyObj->ScanRate = InteractionScanRate;
    MyObj->ScanRange = InteractionScanRange;
    MyObj->ScanCollisionChannel = InteractionCollisionChannel;
    return MyObj;
}

void UAbilityTask_LookForInteractables::Activate()
{
    Super::Activate();

    SetWaitingOnAvatar();

    GetWorld()->GetTimerManager().SetTimer(ScanTimerHandle, this, &ThisClass::PerformTrace, ScanRate, true);
}

void UAbilityTask_LookForInteractables::OnDestroy(bool AbilityEnded)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ScanTimerHandle);
    }

    Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_LookForInteractables::PerformTrace()
{
    APawn* AvatarPawn = Cast<APawn>(Ability->GetCurrentActorInfo()->AvatarActor.Get());
    if (!AvatarPawn)
    {
        return;
    }

    APlayerController* AvatarPlayerController = Cast<APlayerController>(AvatarPawn->GetController());
    if (!AvatarPlayerController)
    {
        return;
    }

    FVector ViewStart;
    FRotator ViewRot;
    AvatarPlayerController->GetPlayerViewPoint(ViewStart, ViewRot);
    FVector ViewEnd = ViewStart + ViewRot.Vector() * ScanRange;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_LookForInteractables), /*bTraceComplex*/ false);
    Params.AddIgnoredActor(AvatarPawn);

    FHitResult ScanTrace;
    GetWorld()->LineTraceSingleByChannel(ScanTrace, ViewStart, ViewEnd, ScanCollisionChannel, Params);

    TScriptInterface<IInteractableTarget> InteractableTarget(ScanTrace.GetActor());
    if (InteractableTarget)
    {
        FGameplayAbilitySpec* InteractionAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(InteractableTarget->GetInteractionAbility());
        InteractableObjectsChanged.Broadcast(InteractionAbilitySpec->Handle);     
    }
}
