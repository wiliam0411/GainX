// GainX, All Rights Reserved

#include "Player/GainXPlayerController.h"
#include "Player/GainXPlayerState.h"
#include "Components/GainXRespawnComponent.h"
#include "GainXGameModeBase.h"
#include "GainXGameInstance.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

AGainXPlayerController::AGainXPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    RespawnComponent = CreateDefaultSubobject<UGainXRespawnComponent>("RespawnComponent");
}

AGainXPlayerState* AGainXPlayerController::GetGainXPlayerState() const
{
    return CastChecked<AGainXPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGainXAbilitySystemComponent* AGainXPlayerController::GetGainXAbilitySystemComponent() const
{
    const auto GainXPlayerState = GetGainXPlayerState();

    if (!GainXPlayerState) return nullptr;
    
    return GainXPlayerState->GetGainXAbilitySystemComponent();
}

void AGainXPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused) 
{
    if (auto GainXASC = GetGainXAbilitySystemComponent())
    {
        GainXASC->ProcessAbilityInput(DeltaTime, bGamePaused);
    }
    Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AGainXPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    if (GetWorld())
    {
        const auto GameMode = Cast<AGainXGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &AGainXPlayerController::OnMatchStateChanged);
        }
    }
}

void AGainXPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void AGainXPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent) return;
    InputComponent->BindAction("PauseGame", IE_Pressed, this, &AGainXPlayerController::OnPauseGame);
    InputComponent->BindAction("Mute", IE_Pressed, this, &AGainXPlayerController::OnMuteSound);
}

void AGainXPlayerController::OnPauseGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void AGainXPlayerController::OnMatchStateChanged(EGainXMatchState State)
{
    if (State == EGainXMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void AGainXPlayerController::OnMuteSound()
{
    if (GetWorld())
    {
        const auto GameInstance = Cast<UGainXGameInstance>(GetWorld()->GetGameInstance());
        if (GameInstance)
        {
            GameInstance->ToggleVolume();
        }
    }
}
