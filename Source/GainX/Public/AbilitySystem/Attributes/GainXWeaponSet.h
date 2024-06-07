// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/GainXAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GainXWeaponSet.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class GAINX_API UGainXWeaponSet : public UGainXAttributeSet
{
    GENERATED_BODY()

public:
    UGainXWeaponSet();

    ATTRIBUTE_ACCESSORS(UGainXWeaponSet, MagazineSize);
    ATTRIBUTE_ACCESSORS(UGainXWeaponSet, MagazineAmmo);
    ATTRIBUTE_ACCESSORS(UGainXWeaponSet, SpareAmmo);

private:
    UPROPERTY(BlueprintReadOnly, Category = "GainX|Weapon", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MagazineSize;

    UPROPERTY(BlueprintReadOnly, Category = "GainX|Weapon", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MagazineAmmo;

    UPROPERTY(BlueprintReadOnly, Category = "GainX|Weapon", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData SpareAmmo;
};
