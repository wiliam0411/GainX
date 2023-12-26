// GainX, All Rights Reserved

#include "Menu/GainXMenuGameModeBase.h"
#include "Menu/GainXMenuPlayerController.h"
#include "Menu/UI/GainXMenuHUD.h"

AGainXMenuGameModeBase::AGainXMenuGameModeBase()
{
    PlayerControllerClass = AGainXMenuPlayerController::StaticClass();
    HUDClass = AGainXMenuHUD::StaticClass();
}
