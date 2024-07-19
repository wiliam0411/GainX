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
class UGainXCameraMode;

UCLASS()
class GAINX_API AGainXPlayerCharacter : public AGainXBaseCharacter
{
    GENERATED_BODY()

public:
    AGainXPlayerCharacter(const FObjectInitializer& ObjInit);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /* The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
    static const FName NAME_BindInputsNow;

    //~AActor interface
    virtual void PostInitializeComponents() override;
    //~End of AActor interface

protected:
    void Input_AbilityInputTagPressed(FGameplayTag InputTag);
    void Input_AbilityInputTagReleased(FGameplayTag InputTag);

    void Move(const FInputActionValue& InputActionValue);
    void Look(const FInputActionValue& InputActionValue);

    TSubclassOf<UGainXCameraMode> DetermineCameraMode() const;
};
