// GainX, All Rights Reserved

#include "Player/GainXPlayerCharacter.h"
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

AGainXPlayerCharacter::AGainXPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AGainXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    auto PlayerController = Cast<APlayerController>(Controller);
    if (!PlayerController) return;

    auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (!Subsystem || !InputMapping) return;

    Subsystem->AddMappingContext(InputMapping, 0);

    auto GainXInputComponent = Cast<UGainXInputComponent>(PlayerInputComponent);
    if (!GainXInputComponent) return;

    // clang-format off
    TArray<uint32> BindHandles;
	GainXInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);

    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AGainXPlayerCharacter::Move);
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AGainXPlayerCharacter::Look);
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &AGainXPlayerCharacter::Jump);
    
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Weapon_Fire, ETriggerEvent::Started, WeaponComponent.Get(), &UGainXWeaponComponent::StartFire);
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Weapon_Fire, ETriggerEvent::Completed, WeaponComponent.Get(), &UGainXWeaponComponent::StopFire);

    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Weapon_Switch_Next, ETriggerEvent::Started, WeaponComponent.Get(), &UGainXWeaponComponent::NextWeapon);
    GainXInputComponent->BindActionByTag(InputConfig, GainXGameplayTags::InputTag_Weapon_Reload, ETriggerEvent::Started, WeaponComponent.Get(), &UGainXWeaponComponent::Reload);
    // clang-format on
}

void AGainXPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    auto GainXPS = Cast<AGainXPlayerState>(Controller->PlayerState);
    InitializeAbilitySystem(GainXPS->GetGainXAbilitySystemComponent(), GainXPS);

    // Bind camera overlap delegates
    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGainXPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AGainXPlayerCharacter::OnCameraCollisionEndOverlap);

    GetGainXPlayerState()->SetAbilitySet(AbilitySet);
}

void AGainXPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
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

void AGainXPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void AGainXPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void AGainXPlayerCharacter::CheckCameraOverlap()
{
    // Hide character mesh if it overlaps camera
    const bool HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    // Get array of character mesh child components 
    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    // Hide child components if it overlaps camera
    for (auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (!MeshChildGeometry) continue;

        MeshChildGeometry->SetOwnerNoSee(HideMesh);
    }
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