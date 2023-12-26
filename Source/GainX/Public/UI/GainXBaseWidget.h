// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GainXBaseWidget.generated.h"

class USoundCue;

UCLASS()
class GAINX_API UGainXBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* OpenSound;
};
