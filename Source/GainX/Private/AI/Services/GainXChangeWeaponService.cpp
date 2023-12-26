// GainX, All Rights Reserved

#include "AI/Services/GainXChangeWeaponService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/GainXWeaponComponent.h"
#include "GainXUtils.h"

UGainXChangeWeaponService::UGainXChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void UGainXChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		const auto WeaponComponent = GainXUtils::GetGainXPlayerComponent<UGainXWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			if (FMath::FRand() < Probability)
			{
				WeaponComponent->NextWeapon();
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
