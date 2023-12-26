// GainX, All Rights Reserved


#include "Pickups/GainXAmmoPickup.h"
#include "Components/GainXWeaponComponent.h"
#include "Components/GainXHealthComponent.h"
#include "GainXUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool AGainXAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = GainXUtils::GetGainXPlayerComponent<UGainXHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = GainXUtils::GetGainXPlayerComponent<UGainXWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
