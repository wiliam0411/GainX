// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/GainXBaseWidget.h"
#include "GainXCoreTypes.h"
#include "GainXGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class GAINX_API UGainXGameOverWidget : public UGainXBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;

	virtual void NativeOnInitialized() override;

private:
	void OnMatchStateChanged(EGainXMatchState State);
	void UpdatePlayerStat();

	UFUNCTION()
	void OnResetLevel();
};
