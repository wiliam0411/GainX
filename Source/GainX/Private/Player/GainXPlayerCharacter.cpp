// GainX, All Rights Reserved

#include "Player/GainXPlayerCharacter.h"
#include "Player/GainXPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/GainXWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/GainXInputComponent.h"
#include "InputAction.h"
#include "GainXGameplayTags.h"
#include "Components/GainXCharacterMovementComponent.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/GainXAbilitySet.h"
#include "Player/GainXPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Camera/GainXCameraComponent.h"

const FName AGainXPlayerCharacter::NAME_BindInputsNow("BindInputsNow");

AGainXPlayerCharacter::AGainXPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGainXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    const auto PlayerController = Cast<APlayerController>(Controller);
    check(PlayerController);

    const auto LocalPlayer = PlayerController->GetLocalPlayer();
    check(LocalPlayer);

    auto Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(Subsystem);

    Subsystem->ClearAllMappings();

    check(PawnData);
    const auto InputConfig = PawnData->InputConfig;
    check(InputConfig);

    auto GainXInputComponent = Cast<UGainXInputComponent>(PlayerInputComponent);
    check(GainXInputComponent);

    TArray<uint32> BindHandles;
    GainXInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);

    // @TODO: review Native Actions
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AGainXPlayerCharacter::Move);
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AGainXPlayerCharacter::Look);

    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PlayerController), NAME_BindInputsNow);
}

void AGainXPlayerCharacter::Move(const FInputActionValue& InputActionValue)
{
    FVector2D MoveVector = InputActionValue.Get<FVector2D>();
    if (Controller)
    {
        AddMovementInput(GetActorForwardVector(), MoveVector.Y);
        AddMovementInput(GetActorRightVector(), MoveVector.X);
    }
}

void AGainXPlayerCharacter::Look(const FInputActionValue& InputActionValue)
{
    FVector2D LookVector = InputActionValue.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);
    }
}

TSubclassOf<UGainXCameraMode> AGainXPlayerCharacter::DetermineCameraMode() const
{
    if (PawnData)
    {
        return PawnData->DefaultCameraMode;
    }

    return nullptr;
}

void AGainXPlayerCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    /* APawn* Pawn = Cast<APawn>(this);

    // Hook up the delegate for all pawns, in case we spectate later
    if (PawnData)
    {
        CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
    } */
}

void AGainXPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (UGainXAbilitySystemComponent* GainXASC = GetGainXAbilitySystemComponent())
    {
        GainXASC->AbilityInputTagPressed(InputTag);
    }
}

void AGainXPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (UGainXAbilitySystemComponent* GainXASC = GetGainXAbilitySystemComponent())
    {
        GainXASC->AbilityInputTagReleased(InputTag);
    }
}