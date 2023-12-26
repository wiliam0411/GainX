// GainX, All Rights Reserved

#include "AI/GainXAIController.h"
#include "AI/GainXAICharacter.h"
#include "Components/GainXAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/GainXRespawnComponent.h"

AGainXAIController::AGainXAIController()
{
    GainXAIPerceptionComponent = CreateDefaultSubobject<UGainXAIPerceptionComponent>("GainXAIPerceptionComponent");
    SetPerceptionComponent(*GainXAIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<UGainXRespawnComponent>("RespawnComponent");

    bWantsPlayerState = true;
}

void AGainXAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto GainXCharacter = Cast<AGainXAICharacter>(InPawn);
    if (GainXCharacter)
    {
        RunBehaviorTree(GainXCharacter->BehaviorTreeAsset);
    }
}

void AGainXAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* AGainXAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
