// GainX, All Rights Reserved

#include "AI/Services/GainXFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/GainXWeaponComponent.h"
#include "GainXUtils.h"

UGainXFireService::UGainXFireService()
{
	NodeName = "Fire";
}

void UGainXFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const bool HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
