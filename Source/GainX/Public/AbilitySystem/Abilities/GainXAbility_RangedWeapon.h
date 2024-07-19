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

public:
    FRangedWeaponFiringInput() : StartTrace(ForceInitToZero), EndAim(ForceInitToZero), AimDir(ForceInitToZero) {}

    FVector StartTrace;

    FVector EndAim;

    FVector AimDir;

    TObjectPtr<UGainXWeaponInstance> EquipmentWeapon;
};

UCLASS(Blueprintable)
class GAINX_API UGainXTracerData : public UObject
{
    GENERATED_BODY()

public:
    UGainXTracerData() {}

    void SetHitPositions(TArray<FVector>& InHitPositions) { HitPositions = InHitPositions; }

    UFUNCTION(BlueprintCallable)
    TArray<FVector> GetHitPositions() const { return HitPositions; }

    void SetHitNormals(TArray<FVector>& InHitNormals) { HitNormals = InHitNormals; }

    UFUNCTION(BlueprintCallable)
    TArray<FVector> GetHitNormals() const { return HitNormals; }

private:
    TArray<FVector> HitPositions;
    TArray<FVector> HitNormals;
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
    /* Does a single weapon trace, either sweeping or ray depending on if SweepRadius is above zero */
    FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, TArray<FHitResult>& OutHitResults) const;

    /* Traces all of the bullets in a single cartridge */
    void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

    void PerformLocalTargeting(TArray<FHitResult>& OutHits);

    void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData);

    FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent);

    UFUNCTION(BlueprintCallable)
    void StartRangedWeaponTargeting();

    UFUNCTION(BlueprintImplementableEvent)
    void OnRangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

    UFUNCTION(BlueprintCallable)
    FGameplayCueParameters MakeGameplayCueParametersFromTargetData(const FGameplayAbilityTargetDataHandle& InData);
};
