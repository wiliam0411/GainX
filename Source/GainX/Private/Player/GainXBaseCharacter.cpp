// GainX, All Rights Reserved

#include "Player/GainXBaseCharacter.h"
#include "Components/GainXCharacterMovementComponent.h"
#include "Components/GainXHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GainXPlayerState.h"
#include "Player/GainXPlayerController.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Player/GainXPawnData.h"
#include "GameModes/GainXExperience.h"
#include "GainXGameModeBase.h"
#include "GameModes/GainXExperienceManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/GainXCameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXCharacter, All, All);

static FName NAME_GainXCharacterCollisionProfile_Capsule(TEXT("GainXPawnCapsule"));
static FName NAME_GainXCharacterCollisionProfile_Mesh(TEXT("GainXPawnMesh"));

AGainXBaseCharacter::AGainXBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UGainXCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    check(CapsuleComp);
    CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
    CapsuleComp->SetCollisionProfileName(NAME_GainXCharacterCollisionProfile_Capsule);

    USkeletalMeshComponent* MeshComp = GetMesh();
    check(MeshComp);
    // Rotate mesh to be X forward since it is exported as Y forward.
    MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    MeshComp->SetCollisionProfileName(NAME_GainXCharacterCollisionProfile_Mesh);

    UGainXCharacterMovementComponent* GainXMovementComp = CastChecked<UGainXCharacterMovementComponent>(GetCharacterMovement());
    GainXMovementComp->GravityScale = 1.0f;
    GainXMovementComp->MaxAcceleration = 2400.0f;
    GainXMovementComp->BrakingFrictionFactor = 1.0f;
    GainXMovementComp->BrakingFriction = 6.0f;
    GainXMovementComp->GroundFriction = 8.0f;
    GainXMovementComp->BrakingDecelerationWalking = 1400.0f;
    GainXMovementComp->bUseControllerDesiredRotation = false;
    GainXMovementComp->bOrientRotationToMovement = false;
    GainXMovementComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
    GainXMovementComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
    GainXMovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
    GainXMovementComp->bCanWalkOffLedgesWhenCrouching = true;
    GainXMovementComp->SetCrouchedHalfHeight(65.0f);

    HealthComponent = CreateDefaultSubobject<UGainXHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);

    CameraComponent = CreateDefaultSubobject<UGainXCameraComponent>(TEXT("DefaultCamera1"));
    CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

    AbilitySystemComponent = CreateDefaultSubobject<UGainXAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    HealthSet = CreateDefaultSubobject<UGainXHealthSet>("HealthSet");

    NetUpdateFrequency = 100.0f;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    BaseEyeHeight = 65.0f;
    CrouchedEyeHeight = 50.0f;
}

UGainXCharacterMovementComponent* AGainXBaseCharacter::GetGainXMovementComponent() const
{
    return CastChecked<UGainXCharacterMovementComponent>(GetCharacterMovement());
}

AGainXPlayerController* AGainXBaseCharacter::GetGainXPlayerController() const
{
    return CastChecked<AGainXPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AGainXPlayerState* AGainXBaseCharacter::GetGainXPlayerState() const
{
    return CastChecked<AGainXPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UGainXAbilitySystemComponent* AGainXBaseCharacter::GetGainXAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAbilitySystemComponent* AGainXBaseCharacter::GetAbilitySystemComponent() const
{
    return GetGainXAbilitySystemComponent();
}

USkeletalMeshComponent* AGainXBaseCharacter::GetFirstPersonMesh_Implementation() const
{
    return nullptr;
}

USceneComponent* AGainXBaseCharacter::GetFirstPersonCamera_Implementation() const
{
    return nullptr;
}

void AGainXBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UninitializeAbilitySystem();

    Super::EndPlay(EndPlayReason);
}

void AGainXBaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    InitializeAbilitySystem();

    if (GetWorld() && GetWorld()->IsGameWorld() && GetWorld()->GetNetMode() != NM_Client)
    {
        AGameStateBase* GameState = GetWorld()->GetGameState();
        check(GameState);

        auto ExperienceComponent = GameState->FindComponentByClass<UGainXExperienceManagerComponent>();
        check(ExperienceComponent);

        ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnGainXExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
    }


}

void AGainXBaseCharacter::BeginPlay() 
{
    Super::BeginPlay();


}

void AGainXBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateEyeHeight(DeltaTime);

    // @TODO: TickComponent function is not working thus we meed call it here
    AbilitySystemComponent->ProcessAbilityInput();
}

void AGainXBaseCharacter::OnConstruction(const FTransform& Transform) 
{
    Super::OnConstruction(Transform);

    // Setup Fisrt person camera boom
    if (GetFirstPersonCamera())
    {
        GetFirstPersonCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
        StancedEyeHeight = BaseEyeHeight;
        CurrentEyeHeight = StancedEyeHeight;
        TargetEyeHeight = CurrentEyeHeight;
    }
}

void AGainXBaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    CurrentEyeHeight = StancedEyeHeight + HalfHeightAdjust;
    TargetEyeHeight = CrouchedEyeHeight;

    if (GetFirstPersonCamera())
    {
        GetFirstPersonCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, CurrentEyeHeight));
    }

    Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AGainXBaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    CurrentEyeHeight = CrouchedEyeHeight - HalfHeightAdjust;
    TargetEyeHeight = StancedEyeHeight;

    if (GetFirstPersonCamera())
    {
        GetFirstPersonCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, CurrentEyeHeight));
    }

    Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AGainXBaseCharacter::OnDeath(AActor* OwningActor)
{
    DisableMovementAndCollision();
    K2_OnDeath();
    UninitializeAbilitySystem();
    DetachFromControllerPendingDestroy();
}

void AGainXBaseCharacter::InitializeAbilitySystem()
{
    check(AbilitySystemComponent);
    check(HealthComponent);

    // Set owner and avatar actor of the ability sysytem component
    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    // Bind health components delegates
    HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void AGainXBaseCharacter::UninitializeAbilitySystem()
{
    check(AbilitySystemComponent);
    check(HealthComponent);

    // Unbind health components delegates
    HealthComponent->UninitializeFromAbilitySystem();

    AbilitySystemComponent->CancelAbilities();
    AbilitySystemComponent->ClearAbilityInput();
    AbilitySystemComponent->RemoveAllGameplayCues();
    AbilitySystemComponent->SetAvatarActor(nullptr);
}

void AGainXBaseCharacter::DisableMovementAndCollision()
{
    if (Controller)
    {
        Controller->SetIgnoreMoveInput(true);
    }

    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    check(CapsuleComp);
    CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

    UGainXCharacterMovementComponent* GainXMovementComp = CastChecked<UGainXCharacterMovementComponent>(GetCharacterMovement());
    GainXMovementComp->StopMovementImmediately();
    GainXMovementComp->DisableMovement();
}

void AGainXBaseCharacter::SetPawnData(const UGainXPawnData* InPawnData)
{
    check(InPawnData);

    if (PawnData)
    {
        UE_LOG(LogGainXCharacter, Error, TEXT("Trying to set PawnData on character that already has valid PawnData."));
        return;
    }

    PawnData = InPawnData;

    for (const auto AbilitySet : PawnData->AbilitySets)
    {
        if (!AbilitySet)
        {
            UE_LOG(LogGainXCharacter, Error, TEXT("Trying to give non-valid ability set to character."));
        }

        AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
    }
}

void AGainXBaseCharacter::OnExperienceLoaded(const UGainXExperience* CurrentExperience)
{
    check(CurrentExperience);

    if (AGainXGameModeBase* GainXGameMode = GetWorld()->GetAuthGameMode<AGainXGameModeBase>())
    {
        if (const UGainXPawnData* NewPawnData = GainXGameMode->GetPawnDataForController(GetController()))
        {
            SetPawnData(NewPawnData);
        }
        else
        {
            UE_LOG(LogGainXCharacter, Error, TEXT("Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
        }
    }
}

void AGainXBaseCharacter::UpdateEyeHeight(float DeltaTime) 
{
    CurrentEyeHeight = FMath::FInterpTo(CurrentEyeHeight, TargetEyeHeight, DeltaTime, CrouchSpeed);
    if (GetFirstPersonCamera())
    {
        GetFirstPersonCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, CurrentEyeHeight));
    }
}
