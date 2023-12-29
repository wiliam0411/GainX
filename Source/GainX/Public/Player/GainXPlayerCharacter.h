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
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    USphereComponent* CameraCollisionComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UGainXInputConfig* InputConfig;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* InputMapping;

    virtual void BeginPlay() override;
    virtual void OnDeath() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual bool IsRunning() const override;

private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    // Enhanced Input
    void Move(const FInputActionValue& InputActionValue);
    void Look(const FInputActionValue& InputActionValue);

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void OnStartRunning();
    void OnStopRunning();

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlap();
};
