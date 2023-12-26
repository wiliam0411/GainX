// GainX, All Rights Reserved

#include "Components/GainXAIPerceptionComponent.h"
#include "AIController.h"
#include "GainXUtils.h"
#include "Components/GainXHealthComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* UGainXAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0)
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
        if (PercieveActors.Num() == 0) return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (const auto PercieveActor : PercieveActors)
    {
        const auto HealthComponenet = GainXUtils::GetGainXPlayerComponent<UGainXHealthComponent>(PercieveActor);

        const auto PercievePawn = Cast<APawn>(PercieveActor);
        const auto AreEnemies = PercievePawn && GainXUtils::AreEnemies(Controller, PercievePawn->Controller);

        if (HealthComponenet && !HealthComponenet->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }
    return BestPawn;
}