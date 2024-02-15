// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GainXCoreTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "GainXHealthComponent.generated.h"

class UCameraShakeBase;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAINX_API UGainXHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGainXHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health) || Health < 0.0f; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return UKismetMathLibrary::SafeDivide(Health, MaxHealth); }

    float GetHealth() const { return Health; }

    float GetMaxHealth() const { return MaxHealth; }

    bool TryToAddHealth(float HealthAmount);

    bool IsHealthFull() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health");
    TMap<TObjectPtr<UPhysicalMaterial>, float> DamageModifaers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelayTime = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealPerSecond = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1", ClampMax = "300.0", EditCondition = "AutoHeal"))
    float HealTimerUpdateRate = 60.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;

    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
        class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
        AActor* DamageCauser);

    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
        const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

    /* Callback function of HealTimerHandle */
    void HealUpdate();

    /* Health update function */
    void SetHealth(float NewHealth);

    void Killed(AController* KillerController);

    /* Apply damage, handling death and heal activation */
    void ApplyDamage(float Damage, AController* InstigatedBy);

    /* Get damage multiplier for body parts from phys material */
    float GetPointDamageModifaer(AActor* DamagedActor, const FName& BoneName);

    // TODO: only for player character
    /* Shake camera when character takes damage */
    void PlayCameraShake();

    /* TODO:for AI */
    void ReportDamageEvent(float Damage, AController* InstigatedBy);
};
