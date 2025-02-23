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

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    USkeletalMeshComponent* GetFirstPersonMesh() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    USceneComponent* GetFirstPersonCamera() const;

    //~AActor interface
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform& Transform) override;
    //~End of AActor interface

    //~ACharacter interface
    virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    //~End of ACharacter interface

    void SetPawnData(const UGainXPawnData* InPawnData);
    const UGainXPawnData* GetPawnData() const { return PawnData; }

protected:
    UFUNCTION()
    virtual void OnDeath(AActor* OwningActor);

    virtual void InitializeAbilitySystem();
    virtual void UninitializeAbilitySystem();

    void DisableMovementAndCollision();

    /* Sets new pawn data when experience is loaded */
    void OnExperienceLoaded(const UGainXExperience* CurrentExperience);

    void UpdateEyeHeight(float DeltaTime);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeath"))
    void K2_OnDeath();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GainX|Character", Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UGainXCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GainX|Character", Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UGainXAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GainX|Character", Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UGainXHealthComponent> HealthComponent;

    UPROPERTY()
    TObjectPtr<const UGainXPawnData> PawnData;

    UPROPERTY()
    TObjectPtr<const class UGainXHealthSet> HealthSet;

    UPROPERTY(EditDefaultsOnly)
    float CrouchSpeed = 15.0f;

private:
    float StancedEyeHeight = 0.0f;
    float CurrentEyeHeight = 0.0f;
    float TargetEyeHeight = 0.0f;
};