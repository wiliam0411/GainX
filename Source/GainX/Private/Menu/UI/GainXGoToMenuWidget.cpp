// GainX, All Rights Reserved

#include "Menu/UI/GainXGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GainXGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXGoToMenuWidget, All, All);

void UGainXGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (GoToMenuButton)
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &UGainXGoToMenuWidget::OnGoToMenu);
    }
}

void UGainXGoToMenuWidget::OnGoToMenu()
{
    if (!GetWorld()) return;

    const auto GainXGameInstance = GetWorld()->GetGameInstance<UGainXGameInstance>();
    if (!GainXGameInstance) return;
}
