// GainX, All Rights Reserved

#include "Menu/UI/GainXMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GainXGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/GainXLevelItemWidget.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXMenuWidget, All, All);

void UGainXMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UGainXMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UGainXMenuWidget::OnQuitGame);
    }

    InitLevelItems();
}

void UGainXMenuWidget::InitLevelItems()
{
    const auto GainXGameInstance = GetGainXGameInstance();
    if (!GainXGameInstance) return;

    //checkf(GainXGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();
    /*
    for (auto LevelData : GainXGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<UGainXLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &UGainXMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }
    if (GainXGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(GainXGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(GainXGameInstance->GetStartupLevel());
    }*/
}

void UGainXMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto GainXGameInstance = GetGainXGameInstance();
    if (!GainXGameInstance) return;

    //GainXGameInstance->SetStartupLevelName(Data);

    for (auto LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

void UGainXMenuWidget::OnStartGame()
{
    PlayAnimation(HideAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UGainXMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation != HideAnimation) return;

    const auto GainXGameInstance = GetGainXGameInstance();
    if (!GainXGameInstance) return;

    //UGameplayStatics::OpenLevel(this, GainXGameInstance->GetStartupLevel().LevelName);
}

void UGainXMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UGainXGameInstance* UGainXMenuWidget::GetGainXGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<UGainXGameInstance>();
}
