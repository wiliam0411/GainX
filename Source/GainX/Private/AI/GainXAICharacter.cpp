// GainX, All Rights Reserved

#include "AI/GainXAICharacter.h"
#include "AI/GainXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/GainXAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/GainXHealthBarWidget.h"
#include "Components/GainXHealthComponent.h"

AGainXAICharacter::AGainXAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UGainXAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AGainXAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 200.0f);
    }
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(RootComponent);
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void AGainXAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateHealthWidgetVisibility();
}

void AGainXAICharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthWidgetComponent);
}

void AGainXAICharacter::OnDeath(AActor* OwningActor)
{
    Super::OnDeath(OwningActor);

    const auto GainXController = Cast<AAIController>(Controller);
    if (GainXController && GainXController->BrainComponent)
    {
        GainXController->BrainComponent->Cleanup();
    }
}

void AGainXAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() ||                                       //
        !GetWorld()->GetFirstPlayerController() ||           //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())  //
        return;
    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
