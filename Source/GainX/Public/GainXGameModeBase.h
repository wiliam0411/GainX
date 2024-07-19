// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "GainXGameModeBase.generated.h"

class UGainXExperience;
class UGainXPawnData;

UCLASS(Config = Game)
class GAINX_API AGainXGameModeBase : public AModularGameModeBase
{
    GENERATED_BODY()

public:
    AGainXGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|Pawn")
    const UGainXPawnData* GetPawnDataForController(const AController* InController) const;

    //~AGameModeBase interface
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
    virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
    virtual void InitGameState() override;
    //~End of AGameModeBase interface

private:
    /* Spawns players when experience is loaded */
    void OnExperienceLoaded(const UGainXExperience* CurrentExperience);

    /* Calls IsExperienceLoaded function of Experience Manager Component */
    bool IsExperienceLoaded() const;

    /* Defines ExperienceId and ExperienceIdSource for OnMatchAssignmentGiven function */
    void HandleMatchAssignmentIfNotExpectingOne();

    /* Sets Experience for ExperienceManagerComponent */
    void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);
};
