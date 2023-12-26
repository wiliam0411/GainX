// GainX, All Rights Reserved

#include "Player/GainXPlayerController.h"
#include "Components/GainXRespawnComponent.h"
#include "GainXGameModeBase.h"
#include "GainXGameInstance.h"

AGainXPlayerController::AGainXPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UGainXRespawnComponent>("RespawnComponent");
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
