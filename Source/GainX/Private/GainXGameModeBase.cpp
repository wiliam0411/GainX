// GainX, All Rights Reserved

#include "GainXGameModeBase.h"
#include "Player/GainXBaseCharacter.h"
#include "Player/GainXPlayerController.h"
#include "UI/GainXGameHUD.h"
#include "AIController.h"
#include "Player/GainXPlayerState.h"
#include "GainXUtils.h"
#include "Components/GainXRespawnComponent.h"
#include "Components/GainXWeaponComponent.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

AGainXGameModeBase::AGainXGameModeBase()
{
    DefaultPawnClass = AGainXBaseCharacter::StaticClass();
    PlayerControllerClass = AGainXPlayerController::StaticClass();
    HUDClass = AGainXGameHUD::StaticClass();
    PlayerStateClass = AGainXPlayerState::StaticClass();
}

void AGainXGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();

    CreateTeamsInfo();

    CurrentRound = 1;

    StartRound();

    SetMatchState(EGainXMatchState::InProgress);
}

UClass* AGainXGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AGainXGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<AGainXPlayerState>(KillerController->PlayerState) : nullptr;

    const auto VictimPlayerState = VictimController ? Cast<AGainXPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeaths();
    }
    StartRespawn(VictimController);
}

void AGainXGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool AGainXGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        StopAllFire();
        SetMatchState(EGainXMatchState::Pause);
    }
    return PauseSet;
}

bool AGainXGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(EGainXMatchState::InProgress);
    }
    return PauseCleared;
}

void AGainXGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i <= GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto GainXAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(GainXAIController);
    }
}

void AGainXGameModeBase::StartRound()
{
    RoundCountdown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &AGainXGameModeBase::GameTimerUpdate, 1.0f, true);
}

void AGainXGameModeBase::GameTimerUpdate()
{
    //UE_LOG(LogGainXGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountdown, CurrentRound, GameData.RoundsNum);
    if (--RoundCountdown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void AGainXGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        ResetOnePlayer(It->Get());
    }
}

void AGainXGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void AGainXGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AGainXPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor AGainXGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    return GameData.DefaultTeamColor;
}

void AGainXGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    const auto Character = Cast<AGainXBaseCharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<AGainXPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void AGainXGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AGainXPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
    }
}

void AGainXGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountdown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!RespawnAvailable) return;

    const auto RespawnComponent = GainXUtils::GetGainXPlayerComponent<UGainXRespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void AGainXGameModeBase::GameOver()
{
    UE_LOG(LogGainXGameModeBase, Display, TEXT("======= GAME OVER ======="));
    LogPlayerInfo();
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    SetMatchState(EGainXMatchState::GameOver);
}

void AGainXGameModeBase::SetMatchState(EGainXMatchState State)
{
    if (MatchState == State) return;
    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}

void AGainXGameModeBase::StopAllFire()
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        const auto WeaponComponent = GainXUtils::GetGainXPlayerComponent<UGainXWeaponComponent>(Pawn);
        if (!WeaponComponent) continue;

        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);
    }
}
