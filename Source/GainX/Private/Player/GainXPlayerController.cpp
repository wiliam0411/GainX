// GainX, All Rights Reserved

#include "Player/GainXPlayerController.h"
#include "Player/GainXPlayerState.h"
#include "Components/GainXRespawnComponent.h"
#include "GainXGameModeBase.h"
#include "GainXGameInstance.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "Equipment/GainXQuickBarComponent.h"
#include "Inventory/GainXInventoryManagerComponent.h"

AGainXPlayerController::AGainXPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    RespawnComponent = CreateDefaultSubobject<UGainXRespawnComponent>("RespawnComponent");
    QuickBarComponent = CreateDefaultSubobject<UGainXQuickBarComponent>("QuickBarComponent");
    InventoryManagerComponent = CreateDefaultSubobject<UGainXInventoryManagerComponent>("InventoryManagerComponent");
}

AGainXPlayerState* AGainXPlayerController::GetGainXPlayerState() const
{
    return CastChecked<AGainXPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void AGainXPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused) 
{
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
