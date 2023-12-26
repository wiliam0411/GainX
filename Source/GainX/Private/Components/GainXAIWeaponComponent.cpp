// GainX, All Rights Reserved

#include "Components/GainXAIWeaponComponent.h"
#include "Weapon/GainXBaseWeapon.h"

void UGainXAIWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	if (CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		CurrentWeapon->StartFire();
	}
}

void UGainXAIWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextIndex != CurrentWeaponIndex)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}
	if (CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		EquipWeapon(CurrentWeaponIndex);
	}
}
