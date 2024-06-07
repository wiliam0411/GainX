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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGainXHealthAttributeChangedDelegate, UGainXHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGainXDeathDelegate, AActor*, OwningActor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAINX_API UGainXHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGainXHealthComponent(const FObjectInitializer& ObjectInitializer);

    /** Delegate fired when the health value has changed */
    UPROPERTY(BlueprintAssignable, Category = "GainX|HealthComponent")
    FGainXHealthAttributeChangedDelegate OnHealthChanged;

    /** Delegate fired when the max health value has changed */
    UPROPERTY(BlueprintAssignable, Category = "GainX|HealthComponent")
    FGainXHealthAttributeChangedDelegate OnMaxHealthChanged;

    /** Delegate fired when the death sequence has started */
    UPROPERTY(BlueprintAssignable, Category = "GainX|HealthComponent")
    FGainXDeathDelegate OnDeath;

    /** Returns the health component if one exists on the specified actor */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    static UGainXHealthComponent* FindHealthComponent(const AActor* Actor);

    /** Initialize the component using an ability system component */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    void InitializeWithAbilitySystem(UGainXAbilitySystemComponent* InASC);

    /** Uninitialize the component, clearing any references to the ability system */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    void UninitializeFromAbilitySystem();

    /** Returns the current health value */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    float GetHealth() const;

    /** Returns the current maximum health value */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    float GetMaxHealth() const;

    /** Returns the current health in the range [0.0, 1.0] */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    float GetHealthNormalized() const;

    /** Returns true if health is nearly zero */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    bool IsDead() const;

    /** Returns true if health is nearly equal max health */
    UFUNCTION(BlueprintCallable, Category = "GainX|HealthComponent")
    bool IsHealthFull() const;

protected:
    virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
    virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
    virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

    /** Called when a component is unregistered */
    virtual void OnUnregister() override;

protected:
    UPROPERTY()
    TObjectPtr<UGainXAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<const UGainXHealthSet> HealthSet;
};