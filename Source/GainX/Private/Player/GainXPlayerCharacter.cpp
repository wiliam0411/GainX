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

    if (auto PlayerController = Cast<APlayerController>(Controller))
    {
        if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (InputMapping)
            {
                Subsystem->AddMappingContext(InputMapping, 0);
            }
        }
    }

    UGainXInputComponent* GainXInputComponent = Cast<UGainXInputComponent>(PlayerInputComponent);

    // Make sure to set your input component class in the InputSettings->DefaultClasses
    check(GainXInputComponent);

    GainXInputComponent->BindActionByTag(
        InputConfig, GainXGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AGainXPlayerCharacter::Move);
    GainXInputComponent->BindActionByTag(
        InputConfig, GainXGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AGainXPlayerCharacter::Look);

    // PlayerInputComponent->BindAxis("MoveForward", this, &AGainXPlayerCharacter::MoveForward);
    // PlayerInputComponent->BindAxis("MoveRight", this, &AGainXPlayerCharacter::MoveRight);
    // PlayerInputComponent->BindAxis("LookUp", this, &AGainXPlayerCharacter::AddControllerPitchInput);
    // PlayerInputComponent->BindAxis("TurnAround", this, &AGainXPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGainXPlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AGainXPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AGainXPlayerCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UGainXWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UGainXWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UGainXWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UGainXWeaponComponent::Reload);

    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &UGainXWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &UGainXWeaponComponent::Zoom, false);
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

void AGainXPlayerCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AGainXPlayerCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AGainXPlayerCharacter::OnStartRunning()
{
    WantsToRun = true;
}

void AGainXPlayerCharacter::OnStopRunning()
{
    WantsToRun = false;
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
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);
    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}

bool AGainXPlayerCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void AGainXPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGainXPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AGainXPlayerCharacter::OnCameraCollisionEndOverlap);
}

void AGainXPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}