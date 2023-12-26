// GainX, All Rights Reserved

#include "AI/Services/GainXFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GainXUtils.h"
#include "Components/GainXAIPerceptionComponent.h"

UGainXFindEnemyService::UGainXFindEnemyService()
{
	NodeName = "Find Enemy";
}

void UGainXFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Display, TEXT("Find Enemy"));
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = GainXUtils::GetGainXPlayerComponent<UGainXAIPerceptionComponent>(Controller);
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
