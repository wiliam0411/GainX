// GainX, All Rights Reserved

#include "Menu/UI/GainXLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UGainXLevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectedButton)
    {
        LevelSelectedButton->OnClicked.AddDynamic(this, &UGainXLevelItemWidget::OnLevelItemClicked);
        LevelSelectedButton->OnHovered.AddDynamic(this, &UGainXLevelItemWidget::OnLevelItemHovered);
        LevelSelectedButton->OnUnhovered.AddDynamic(this, &UGainXLevelItemWidget::OnLevelItemUnhovered);
    }
}

void UGainXLevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void UGainXLevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumbnail);
    }
}

void UGainXLevelItemWidget::SetSelected(bool IsSelected)
{
    if (LevelImage)
    {
        LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
    }
}

void UGainXLevelItemWidget::OnLevelItemHovered()
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void UGainXLevelItemWidget::OnLevelItemUnhovered()
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(ESlateVisibility::Hidden);
    }
}
