// GainX, All Rights Reserved

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GainXPlayerState.generated.h"

class UAbilitySystemComponent;
class UGainXAbilitySystemComponent;
class UGainXAbilitySet;

UCLASS(Config = Game)
class GAINX_API AGainXPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGainXPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void SetTeamID(int32 ID) { TeamID = ID; }
    int32 GetTeamID() const { return TeamID; }

    void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }
    FLinearColor GetTeamColor() const { return TeamColor; }

    void AddKill() { ++KillsNum; }
    int32 GetKillsNum() const { return KillsNum; }

    void AddDeaths() { ++DeathsNum; }
    int32 GetDeathsNum() const { return DeathsNum; }

    UFUNCTION(BlueprintCallable, Category = "GainX|PlayerState")
    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponent() { return AbilitySystemComponent; }
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    void SetAbilitySet(const UGainXAbilitySet* InAbilitySet);

    virtual void PostInitializeComponents() override;

private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;

    UPROPERTY(VisibleAnywhere, Category = "GainX|PlayerState")
    TObjectPtr<UGainXAbilitySystemComponent> AbilitySystemComponent;
};
