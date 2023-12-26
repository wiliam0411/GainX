// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GainXProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UGainXWeaponFXComponent;

UCLASS()
class GAINX_API AGainXProjectile : public AActor
{
    GENERATED_BODY()

public:
    AGainXProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon");
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon");
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    UGainXWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon");
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon");
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon");
    bool DoFullDamage = true;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon");
    float LifeSeconds = 5.0f;

    virtual void BeginPlay() override;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    AController* GetController() const;
};
