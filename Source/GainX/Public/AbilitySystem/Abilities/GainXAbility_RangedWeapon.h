// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GainXAbility_FromEquipment.h"
#include "GainXAbility_RangedWeapon.generated.h"

class UGainXWeaponInstance;

USTRUCT()
struct FRangedWeaponFiringInput
{
    GENERATED_BODY()

    // Start of the trace
    FVector StartTrace;

    // End of the trace if aim were perfect
    FVector EndAim;

    // The direction of the trace if aim were perfect
    FVector AimDir;

    // The weapon instance / source of weapon data
    UGainXWeaponInstance* WeaponData = nullptr;

    // Can we play bullet FX for hits during this trace
    bool bCanPlayBulletFX = false;

    FRangedWeaponFiringInput() : StartTrace(ForceInitToZero), EndAim(ForceInitToZero), AimDir(ForceInitToZero) {}
};

UCLASS()
class GAINX_API UGainXAbility_RangedWeapon : public UGainXAbility_FromEquipment
{
    GENERATED_BODY()

public:
    UGainXAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    UGainXWeaponInstance* GetWeaponInstance() const;

protected:
    /** Does a single weapon trace, either sweeping or ray depending on if SweepRadius is above zero */
    FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, TArray<FHitResult>& OutHitResults) const;

    /** Traces all of the bullets in a single cartridge */
    void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

    void PerformLocalTargeting(TArray<FHitResult>& OutHits);

    UFUNCTION(BlueprintCallable)
    void StartRangedWeaponTargeting(TArray<FHitResult>& OutHits);
};
