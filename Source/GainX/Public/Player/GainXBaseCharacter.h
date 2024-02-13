// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GainXBaseCharacter.generated.h"

class UGainXHealthComponent;
class UGainXWeaponComponent;
class USoundCue;

UCLASS()
class GAINX_API AGainXBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AGainXBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<UGainXHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<UGainXWeaponComponent> WeaponComponent;

    /* The time the body disappears after death */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    // TODO: переделать систему получения урона от падения
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    /* Material parameter name for the color */
    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

    /* Death sound cue*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> DeathSound;

    virtual void BeginPlay() override;

    /* Callback function of OnDeath */
    virtual void OnDeath();

    /* Callback function of OnHealthChanged */
    virtual void OnHealthChanged(float Health, float HealthDelta);

public:
    virtual void TurnOff() override;
    virtual void Reset() override;

    /* Creates and sets up material instance of character mesh */
    void SetPlayerColor(const FLinearColor& Color);

private:
    /* Callback function of LandedDelegate */
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
