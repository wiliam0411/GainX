// GainX, All Rights Reserved

#include "Player/GainXPlayerController.h"
#include "Player/GainXPlayerState.h"
#include "Components/GainXRespawnComponent.h"
#include "GainXGameModeBase.h"
#include "GainXGameInstance.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "Equipment/GainXQuickBarComponent.h"
#include "Inventory/GainXInventoryManagerComponent.h"
#include "Player/GainXBaseCharacter.h"
#include "UI/GainXHUD.h"
#include "Camera/GainXPlayerCameraManager.h"

AGainXPlayerController::AGainXPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
    PlayerCameraManagerClass = AGainXPlayerCameraManager::StaticClass();
}

AGainXPlayerState* AGainXPlayerController::GetGainXPlayerState() const
{
    return CastChecked<AGainXPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGainXAbilitySystemComponent* AGainXPlayerController::GetGainXAbilitySystemComponent() const
{
    const auto GainXCharacter = Cast<AGainXBaseCharacter>(GetPawn());
    if (!GainXCharacter)
    {
        return nullptr;
    }
    return GainXCharacter->GetGainXAbilitySystemComponent();
}

AGainXHUD* AGainXPlayerController::GetGainXHUD() const
{
    return CastChecked<AGainXHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}
