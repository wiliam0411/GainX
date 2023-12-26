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
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto WeaponComponent = GainXUtils::GetGainXPlayerComponent<UGainXWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent) return false;
    
    return WeaponComponent->NeedAmmo(WeaponType);
}