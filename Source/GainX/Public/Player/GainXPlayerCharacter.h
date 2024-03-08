// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/GainXBaseCharacter.h"
#include "GainXPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UGainXInputConfig;
class UInputMappingContext;
class UGainXAbilitySet;
struct FInputActionValue;
struct FGameplayTag;

UCLASS()
class GAINX_API AGainXPlayerCharacter : public AGainXBaseCharacter
{
    GENERATED_BODY()

public:
    AGainXPlayerCharacter(const FObjectInitializer& ObjInit);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UGainXAbilitySet> AbilitySet;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    TObjectPtr<USphereComponent> CameraCollisionComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UGainXInputConfig> InputConfig;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    /** Bind camera overlap delegates */
    virtual void BeginPlay() override;

    /** Switch player to observer after death */
    virtual void OnDeath() override;

    void Input_AbilityInputTagPressed(FGameplayTag InputTag);
    void Input_AbilityInputTagReleased(FGameplayTag InputTag);

private:
    /** Enhanced input handler function */
    void Move(const FInputActionValue& InputActionValue);

    /** Enhanced input handler function */
    void Look(const FInputActionValue& InputActionValue);

    /** Callback function of OnComponentBeginOverlap delegate */
    UFUNCTION()
    void OnCameraCollisionBeginOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Callback function of OnComponentEndOverlap delegate */
    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /** Hide charcter mesh and its child components if overlap camera */
    void CheckCameraOverlap();
};
