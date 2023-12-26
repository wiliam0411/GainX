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

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto WeaponComponent = GainXUtils::GetGainXPlayerComponent<UGainXWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
