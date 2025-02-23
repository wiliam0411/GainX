// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GainXBaseWidget.generated.h"

class USoundCue;

UCLASS()
class GAINX_API UGainXBaseWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    /* @TODO: Deperecated */
	void Show();

	virtual void NativeOnInitialized() override;
	
protected:
    void HandleActorExtension(AActor* Actor, FName EventName);

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* OpenSound;
};
