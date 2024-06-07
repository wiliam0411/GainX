// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Equipment/GainXEquipmentInstance.h"
#include "Equipment/GainXEquipmentObject.h"
#include "GainXWeaponInstance.generated.h"

UCLASS()
class GAINX_API UGainXWeaponInstance : public UGainXEquipmentObject
{
    GENERATED_BODY()

public:
    UGainXWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~UGainXEquipmentInstance interface
    virtual void OnEquipped();
    virtual void OnUnequipped();
    //~End of UGainXEquipmentInstance interface

    void AddSpread();
    void UpdateWeaponInstance(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    int32 GetBulletsPerCartridge() const { return BulletsPerCartridge; }

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    float GetFireRate() const { return FireRate; }

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    float GetFireInterval() const { return 60.0f / FireRate; }

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    float GetCalculatedSpreadAngle() const { return CurrentSpreadAngle; }

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    float GetCalculatedSpreadAngleMultiplier() const { return bHasFirstShotAccuracy ? 0.0f : CurrentSpreadAngleMultiplier; }

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    float GetSpreadExponent() const { return SpreadExponent; }

    UFUNCTION(BlueprintCallable, Category = "GainX|WeaponInstance")
    float GetMaxDamageRange() const { return MaxDamageRange; }

protected:
    // Spread exponent, affects how tightly shots will cluster around the center line
    // when the weapon has spread (non-perfect accuracy). Higher values will cause shots
    // to be closer to the center (default is 1.0 which means uniformly within the spread range)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.1), Category = "Weapon Instance|Spread|Fire Params")
    float SpreadExponent = 1.0f;

    // A curve that maps the heat to the spread angle
    // The X range of this curve typically sets the min/max heat range of the weapon
    // The Y range of this curve is used to define the min and maximum spread angle
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Spread|Fire Params")
    FRuntimeFloatCurve HeatToSpreadCurve;

    // A curve that maps the current heat to the amount a single shot will further 'heat up'
    // This is typically a flat curve with a single data point indicating how much heat a shot adds,
    // but can be other shapes to do things like punish overheating by adding progressively more heat.
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Spread|Fire Params")
    FRuntimeFloatCurve HeatToHeatPerShotCurve;

    // A curve that maps the current heat to the heat cooldown rate per second
    // This is typically a flat curve with a single data point indicating how fast the heat
    // wears off, but can be other shapes to do things like punish overheating by slowing down
    // recovery at high heat.
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Spread|Fire Params")
    FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

    // Time since firing before spread cooldown recovery begins (in seconds)
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Spread|Fire Params", meta = (ForceUnits = s))
    float SpreadRecoveryCooldownDelay = 0.0f;

    // Should the weapon have perfect accuracy when both player and weapon spread are at their minimum value
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Spread|Fire Params")
    bool bAllowFirstShotAccuracy = false;

    // Multiplier when in an aiming camera mode
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Spread|Player Params", meta = (ForceUnits = x))
    float SpreadAngleMultiplier_Aiming = 1.0f;

    // Multiplier when standing still or moving very slowly
    // (starts to fade out at StandingStillSpeedThreshold, and is gone completely by StandingStillSpeedThreshold + StandingStillToMovingSpeedRange)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params", meta = (ForceUnits = x))
    float SpreadAngleMultiplier_StandingStill = 1.0f;

    // Rate at which we transition to/from the standing still accuracy (higher values are faster, though zero is instant; @see FInterpTo)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params")
    float TransitionRate_StandingStill = 5.0f;

    // Speeds at or below this are considered standing still
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params", meta = (ForceUnits = "cm/s"))
    float StandingStillSpeedThreshold = 80.0f;

    // Speeds no more than this above StandingStillSpeedThreshold are used to feather down the standing still bonus until it's back to 1.0
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params", meta = (ForceUnits = "cm/s"))
    float StandingStillToMovingSpeedRange = 20.0f;

    // Multiplier when crouching, smoothly blended to based on TransitionRate_Crouching
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params", meta = (ForceUnits = x))
    float SpreadAngleMultiplier_Crouching = 1.0f;

    // Rate at which we transition to/from the crouching accuracy (higher values are faster, though zero is instant; @see FInterpTo)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params")
    float TransitionRate_Crouching = 5.0f;

    // Spread multiplier while jumping/falling, smoothly blended to based on TransitionRate_JumpingOrFalling
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params", meta = (ForceUnits = x))
    float SpreadAngleMultiplier_JumpingOrFalling = 1.0f;

    // Rate at which we transition to/from the jumping/falling accuracy (higher values are faster, though zero is instant; @see FInterpTo)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Spread|Player Params")
    float TransitionRate_JumpingOrFalling = 5.0f;

    // Number of bullets to fire in a single cartridge (typically 1, but may be more for shotguns)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Weapon Config")
    int32 BulletsPerCartridge = 1;

    // Number of cartridges the weapon fires per minute
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Weapon Config")
    float FireRate = 300.0f;

    // The maximum distance at which this weapon can deal damage
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Weapon Config", meta = (ForceUnits = cm))
    float MaxDamageRange = 25000.0f;

    // The radius for bullet traces sweep spheres (0.0 will result in a line trace)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Instance|Weapon Config", meta = (ForceUnits = cm))
    float BulletTraceSweepRadius = 0.0f;

    // A curve that maps the distance (in cm) to a multiplier on the base damage from the associated gameplay effect
    // If there is no data in this curve, then the weapon is assumed to have no falloff with distance
    UPROPERTY(EditAnywhere, Category = "Weapon Instance|Weapon Config")
    FRuntimeFloatCurve DistanceDamageFalloff;

private:
    // Time since this weapon was last fired (relative to world time)
    double LastFireTime = 0.0;

    // The current heat
    float CurrentHeat = 0.0f;

    // The current spread angle (in degrees, diametrical)
    float CurrentSpreadAngle = 0.0f;

    // Do we currently have first shot accuracy?
    bool bHasFirstShotAccuracy = false;

    // The current *combined* spread angle multiplier
    float CurrentSpreadAngleMultiplier = 1.0f;

    // The current standing still multiplier
    float StandingStillMultiplier = 1.0f;

    // The current jumping/falling multiplier
    float JumpFallMultiplier = 1.0f;

    // The current crouching multiplier
    float CrouchingMultiplier = 1.0f;

    void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
    void ComputeHeatRange(float& MinHeat, float& MaxHeat);

    float ClampHeat(float NewHeat);

    // Updates the spread and returns true if the spread is at minimum
    bool UpdateSpread(float DeltaSeconds);

    // Updates the multipliers and returns true if they are at minimum
    bool UpdateMultipliers(float DeltaSeconds);
};
