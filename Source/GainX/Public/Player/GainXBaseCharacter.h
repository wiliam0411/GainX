// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GainXBaseCharacter.generated.h"

class AGainXPlayerController;
class UGainXHealthComponent;

class AGainXPlayerState;
class UGainXAbilitySystemComponent;
class UGainXPawnData;
class UGainXAbilitySet;
class UGainXEquipmentManagerComponent;
class UGainXExperience;
class UCameraComponent;
class UGainXCameraComponent;

UCLASS(Config = Game)
class GAINX_API AGainXBaseCharacter : public AModularCharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGainXBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    UGainXCharacterMovementComponent* GetGainXMovementComponent() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    AGainXPlayerController* GetGainXPlayerController() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    AGainXPlayerState* GetGainXPlayerState() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponent() const;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UFUNCTION(BlueprintCallable, Category = "GainX|Character")
    UGainXHealthComponent* GetHealthComponent() const { return HealthComponent; }

    //~AActor interface
    virtual void PostInitializeComponents() override;
    virtual void Tick(float DeltaTime) override;
    //~End of AActor interface

    void SetPawnData(const UGainXPawnData* InPawnData);
    const UGainXPawnData* GetPawnData() const { return PawnData; }

protected:
    /* Callback function of OnDeath */
    UFUNCTION()
    virtual void OnDeath(AActor* OwningActor);

    void DisableMovementAndCollision();

    void OnExperienceLoaded(const UGainXExperience* CurrentExperience);

protected:
    // TODO: Use camera modes instead
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GainX|Character", Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GainX|Character", Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UGainXAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GainX|Character", Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UGainXHealthComponent> HealthComponent;

    UPROPERTY()
    TObjectPtr<const UGainXPawnData> PawnData;

    UPROPERTY()
    TObjectPtr<const class UGainXHealthSet> HealthSet;
};