// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GainXCoreTypes.h"
#include "AbilitySystem/Attributes/GainXHealthSet.h"
#include "GainXHealthComponent.generated.h"

class AGainXBaseCharacter;
class UCameraShakeBase;
class UPhysicalMaterial;
class UGainXAbilitySystemComponent;
class UGainXHealthSet;
struct FGameplayEffectSpec;

DECLARE_MULTICAST_DELEGATE_FourParams(FGainXHealth_AttributeChanged, UGainXHealthComponent*, float, float, AActor*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAINX_API UGainXHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGainXHealthComponent(const FObjectInitializer& ObjectInitializer);

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    /** Returns the health component if one exists on the specified actor */
    UFUNCTION(BlueprintCallable, Category = "GainX|Health")
    static UGainXHealthComponent* FindHealthComponent(const AActor* Actor);

    /** Initialize the component using an ability system component */
    UFUNCTION(BlueprintCallable, Category = "GainX|Health")
    void InitializeWithAbilitySystem(UGainXAbilitySystemComponent* InASC);

    /** Uninitialize the component, clearing any references to the ability system */
    UFUNCTION(BlueprintCallable, Category = "GainX|Health")
    void UninitializeFromAbilitySystem();

    /** Returns the current health value */
    UFUNCTION(BlueprintCallable, Category = "GainX|Health")
    float GetHealth() const;

    /** Returns the current maximum health value */
    UFUNCTION(BlueprintCallable, Category = "GainX|Health")
    float GetMaxHealth() const;

    /** Returns the current health in the range [0.0, 1.0] */
    UFUNCTION(BlueprintCallable, Category = "GainX|Health")
    float GetHealthNormalized() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

    bool IsHealthFull() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health");
    TMap<TObjectPtr<UPhysicalMaterial>, float> DamageModifaers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
    virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
    virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

    virtual void OnUnregister() override;

private:
    /* Calls killed method of GameMode class */
    void Killed(AController* KillerController);

    /* Get damage multiplier for body parts from phys material */
    float GetPointDamageModifaer(AActor* DamagedActor, const FName& BoneName);

    /* Shake camera when character takes damage */
    void PlayCameraShake();

    /* Triggers AI damage sense  */
    void ReportDamageEvent(float Damage, AController* InstigatedBy);

protected:
    UPROPERTY()
    TObjectPtr<UGainXAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<const UGainXHealthSet> HealthSet;
};
