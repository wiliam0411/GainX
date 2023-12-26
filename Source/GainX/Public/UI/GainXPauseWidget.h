// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/GainXBaseWidget.h"
#include "GainXPauseWidget.generated.h"

class UButton;

UCLASS()
class GAINX_API UGainXPauseWidget : public UGainXBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClearPause();
};
