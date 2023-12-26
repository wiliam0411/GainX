// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/GainXBaseWidget.h"
#include "GainXCoreTypes.h"
#include "GainXMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UGainXGameInstance;
class UGainXLevelItemWidget;
class USoundCue;

UCLASS()
class GAINX_API UGainXMenuWidget : public UGainXBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* HideAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* StartGameSound;

    virtual void NativeOnInitialized() override;

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override ;

private:
    TArray<UGainXLevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);

    UGainXGameInstance* GetGainXGameInstance() const;
};
