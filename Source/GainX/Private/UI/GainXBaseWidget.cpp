// GainX, All Rights Reserved

#include "UI/GainXBaseWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GameFrameworkComponentManager.h"

void UGainXBaseWidget::Show()
{
    PlayAnimation(ShowAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}

void UGainXBaseWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();
}

void UGainXBaseWidget::HandleActorExtension(AActor* Actor, FName EventName) {}
