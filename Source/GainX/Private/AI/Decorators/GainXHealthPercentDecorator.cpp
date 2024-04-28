// GainX, All Rights Reserved

#include "AI/Decorators/GainXHealthPercentDecorator.h"
#include "AIController.h"
#include "GainXUtils.h"
#include "Components/GainXHealthComponent.h"

UGainXHealthPercentDecorator::UGainXHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool UGainXHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HealthComponent = GainXUtils::GetGainXPlayerComponent<UGainXHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthNormalized() <= HealthPercent;
}