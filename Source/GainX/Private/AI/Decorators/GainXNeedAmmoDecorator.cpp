// GainX, All Rights Reserved

#include "AI/Decorators/GainXNeedAmmoDecorator.h"
#include "AIController.h"
#include "GainXUtils.h"
#include "Components/GainXWeaponComponent.h"
#include "Weapon/GainXBaseWeapon.h"

UGainXNeedAmmoDecorator::UGainXNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool UGainXNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return false;
}