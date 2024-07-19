// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/GainXActivatableWidget.h"
#include "GainXHUDLayout.generated.h"

/**
 * Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS(BlueprintType, Blueprintable, Meta = (DisplayName = "GainX HUD Layout", Category = "GainX|HUD"))
class GAINX_API UGainXHUDLayout : public UGainXActivatableWidget
{
    GENERATED_BODY()

public:
    UGainXHUDLayout(const FObjectInitializer& ObjectInitializer);
};