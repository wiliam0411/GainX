// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GainXPlayerController.generated.h"

class UGainXRespawnComponent;
class UGainXQuickBarComponent;
class UGainXInventoryManagerComponent;
class AGainXHUD;
class AGainXPlayerState;
class UGainXAbilitySystemComponent;

UCLASS(Config = Game)
class GAINX_API AGainXPlayerController : public ACommonPlayerController
{
    GENERATED_BODY()

public:
    AGainXPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|PlayerController")
    AGainXPlayerState* GetGainXPlayerState() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|PlayerController")
    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponent() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|PlayerController")
    AGainXHUD* GetGainXHUD() const;
};