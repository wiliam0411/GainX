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

    /* Time the body disappears after death */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    /* Death sound cue*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    TObjectPtr<USoundCue> DeathSound;

    /* Fall velociry at which character takes damage */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float FallDamageVelocityThreshold = 900.0f;

    /* Fall damage multiplier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float FallDamageMultiplier = 1.0f;

    /* Material instance parameter name for the color */
    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorParameterName = "Paint Color";

    /* Setting health and binding delegates */
    virtual void BeginPlay() override;

    /* Callback function of OnDeath */
    virtual void OnDeath();

    /* Callback function of OnHealthChanged */
    virtual void OnHealthChanged(float Health, float HealthDelta);

public:
    virtual void TurnOff() override;
    virtual void Reset() override;

    /* Creates and customize material instance of character mesh */
    void SetPlayerColor(const FLinearColor& Color);

private:
    /* Callback function of LandedDelegate */
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
