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

    // TODO: Should be impl by ability attributes: return HealthComponent->TryToAddHealth(HealthAmount);
    return false;
}
