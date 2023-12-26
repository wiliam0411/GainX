// GainX, All Rights Reserved

#include "Menu/GainXMenuPlayerController.h"

void AGainXMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
