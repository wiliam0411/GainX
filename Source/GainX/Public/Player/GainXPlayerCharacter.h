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
struct FInputActionValue;

UCLASS()
class GAINX_API AGainXPlayerCharacter : public AGainXBaseCharacter
{
    GENERATED_BODY()

public:
    AGainXPlayerCharacter(const FObjectInitializer& ObjInit);

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

    virtual void BeginPlay() override;
    virtual void OnDeath() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Enhanced Input
    void Move(const FInputActionValue& InputActionValue);
    void Look(const FInputActionValue& InputActionValue);

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlap();
};
