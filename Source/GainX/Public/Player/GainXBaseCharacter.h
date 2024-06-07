// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "Player/GainXPlayerState.h"

#include "GainXBaseCharacter.generated.h"

class UGainXHealthComponent;
class UGainXWeaponComponent;
class USoundCue;
class UGainXAbilitySystemComponent;
class AGainXPlayerState;
class UGainXAbilitySet;
class UGainXEquipmentManagerComponent;

UCLASS()
class GAINX_API AGainXBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGainXBaseCharacter(const FObjectInitializer& ObjInit);

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    UGainXHealthComponent* GetHealthComponent() const { return HealthComponent; }

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    UGainXWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    AGainXPlayerState* GetGainXPlayerState() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponent() const;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "GainX|PlayerState")
    TObjectPtr<UGainXAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<UGainXHealthComponent> HealthComponent;

    // TODO: Should be impl by gamefeatures
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<UGainXEquipmentManagerComponent> EquipmentManagerComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TObjectPtr<UGainXAbilitySet> AbilitySet;

    UPROPERTY()
    TObjectPtr<const class UGainXHealthSet> HealthSet;

    /** Delegate fired when our pawn becomes the ability system's avatar actor */
    FSimpleMulticastDelegate OnAbilitySystemInitialized;

    /* Deprecated */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<UGainXWeaponComponent> WeaponComponent;

    /** Time the body disappears after death */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    /** Death sound cue */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    TObjectPtr<USoundCue> DeathSound;

    /** Material instance parameter name for the color */
    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorParameterName = "Paint Color";

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** Callback function of OnDeath */
    UFUNCTION()
    virtual void OnDeath(AActor* OwningActor);

public:
    /** Creates and customize material instance of character mesh */
    void SetPlayerColor(const FLinearColor& Color);
};
