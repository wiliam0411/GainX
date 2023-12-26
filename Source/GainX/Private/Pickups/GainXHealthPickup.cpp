// GainX, All Rights Reserved


#include "Pickups/GainXHealthPickup.h"
#include "Components/GainXHealthComponent.h"
#include "GainXUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool AGainXHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    UE_LOG(LogHealthPickup, Display, TEXT("Health was taken"));

    const auto HealthComponent = GainXUtils::GetGainXPlayerComponent<UGainXHealthComponent>(PlayerPawn);
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}
