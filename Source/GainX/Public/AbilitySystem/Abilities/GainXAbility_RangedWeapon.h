// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GainXAbility_FromEquipment.h"
#include "GainXAbility_RangedWeapon.generated.h"

class AGainXWeaponActor;

UCLASS()
class GAINX_API UGainXAbility_RangedWeapon : public UGainXAbility_FromEquipment
{
    GENERATED_BODY()

public:
    UGainXAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    AGainXWeaponActor* GetWeaponActor() const;

    /* Does single trace and returns first hit */
    FHitResult TraceSingleBullet(const FVector& StartTrace, const FVector& EndTrace) const;

    /* Calculates spread and trace all bullets in cartrige */
    void TraceCartridge(FVector StartTrace, FVector AimDirection, TArray<FHitResult>& OutHits);

    /* Determines start trace point and aim direction */
    void PerformLocalTargeting(TArray<FHitResult>& OutHits);

    /* Core of spread calculation */
    FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent);

    UFUNCTION(BlueprintCallable)
    void StartRangedWeaponTargeting();

    UFUNCTION(BlueprintImplementableEvent)
    void OnRangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

    UFUNCTION(BlueprintCallable)
    void GetHitResultsFromTargetData(const FGameplayAbilityTargetDataHandle& InData, TArray<FHitResult>& HitResults) const;
};
