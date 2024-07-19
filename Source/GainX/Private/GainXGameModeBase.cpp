// GainX, All Rights Reserved

#include "GainXGameModeBase.h"
#include "Player/GainXBaseCharacter.h"
#include "Player/GainXPlayerController.h"
#include "Player/GainXPlayerState.h"
#include "Player/GainXPawnData.h"
#include "UI/GainXHUD.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/GainXExperience.h"
#include "GameModes/GainXGameState.h"
#include "GameModes/GainXWorldSettings.h"
#include "GameModes/GainXExperienceManagerComponent.h"
#include "System/GainXAssetManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXGameMode, All, All);

AGainXGameModeBase::AGainXGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    GameStateClass = AGainXGameState::StaticClass();
    PlayerStateClass = AGainXPlayerState::StaticClass();
    DefaultPawnClass = AGainXBaseCharacter::StaticClass();
    PlayerControllerClass = AGainXPlayerController::StaticClass();
    HUDClass = AGainXHUD::StaticClass();
}

const UGainXPawnData* AGainXGameModeBase::GetPawnDataForController(const AController* InController) const
{
    // See if pawn data is already set on the player character
    if (InController)
    {
        if (const auto GainXCharacter = Cast<AGainXBaseCharacter>(InController->GetCharacter()))
        {
            if (const UGainXPawnData* PawnData = GainXCharacter->GetPawnData())
            {
                return PawnData;
            }
        }
    }

    // If not, fall back to the the default for the current experience
    check(GameState);
    UGainXExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UGainXExperienceManagerComponent>();
    check(ExperienceComponent);

    if (ExperienceComponent->IsExperienceLoaded())
    {
        const UGainXExperience* Experience = ExperienceComponent->GetCurrentExperienceChecked();
        if (Experience->DefaultPawnData != nullptr)
        {
            return Experience->DefaultPawnData;
        }
    }

    // Experience not loaded yet, so there is no pawn data to be had
    return nullptr;
}

void AGainXGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Wait for the next frame to give time to initialize startup settings
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

UClass* AGainXGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (const UGainXPawnData* PawnData = GetPawnDataForController(InController))
    {
        if (PawnData->PawnClass)
        {
            return PawnData->PawnClass;
        }
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AGainXGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = GetInstigator();
    SpawnInfo.ObjectFlags |= RF_Transient;  // Never save the default player pawns into a map.
    SpawnInfo.bDeferConstruction = true;

    if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
    {
        if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
        {
            SpawnedPawn->FinishSpawning(SpawnTransform);
            return SpawnedPawn;
        }
        else
        {
            UE_LOG(LogGainXGameMode, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
        }
    }
    else
    {
        UE_LOG(LogGainXGameMode, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
    }

    return nullptr;
}

void AGainXGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    // Delay starting new players until the experience has been loaded
    // (players who log in prior to that will be started by OnExperienceLoaded)
    if (IsExperienceLoaded())
    {
        Super::HandleStartingNewPlayer_Implementation(NewPlayer);
    }
}

void AGainXGameModeBase::InitGameState()
{
    Super::InitGameState();

    // Listen for the experience load to complete
    auto ExperienceComponent = GameState->FindComponentByClass<UGainXExperienceManagerComponent>();
    check(ExperienceComponent);
    ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnGainXExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AGainXGameModeBase::OnExperienceLoaded(const UGainXExperience* CurrentExperience)
{
    // Spawn any players that are already attached
    //@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
    // GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PlayerController = Cast<APlayerController>(*Iterator);
        if (PlayerController && !PlayerController->GetPawn())
        {
            if (PlayerCanRestart(PlayerController))
            {
                RestartPlayer(PlayerController);
            }
        }
    }
}

bool AGainXGameModeBase::IsExperienceLoaded() const
{
    check(GameState);
    auto ExperienceManagerComponent = GameState->FindComponentByClass<UGainXExperienceManagerComponent>();
    check(ExperienceManagerComponent);
    return ExperienceManagerComponent->IsExperienceLoaded();
}

void AGainXGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
    FPrimaryAssetId ExperienceId;
    FString ExperienceIdSource;

    if (AGainXWorldSettings* GainXWorldSettings = Cast<AGainXWorldSettings>(GetWorldSettings()))
    {
        ExperienceId = GainXWorldSettings->GetDefaultGameplayExperience();
        ExperienceIdSource = TEXT("WorldSettings");
    }

    OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void AGainXGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
    if (ExperienceId.IsValid())
    {
        UE_LOG(LogGainXGameMode, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

        UGainXExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGainXExperienceManagerComponent>();
        check(ExperienceManagerComponent);
        ExperienceManagerComponent->SetCurrentExperience(ExperienceId);
    }
    else
    {
        UE_LOG(LogGainXGameMode, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
    }
}
