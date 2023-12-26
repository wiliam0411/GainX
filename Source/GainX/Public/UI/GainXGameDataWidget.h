// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GainXGameDataWidget.generated.h"

class AGainXGameModeBase;
class AGainXPlayerState;

UCLASS()
class GAINX_API UGainXGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetCurrentRoundNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetTotalRoundsNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetRoundSecondRemaining() const;

private:
    AGainXGameModeBase* GetGainXGameModeBase() const;
    AGainXPlayerState* GetGainXPlayerState() const;
};
