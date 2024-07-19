// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GainXReticleWidget.generated.h"

class UGainXWeaponInstance;
class UGainXInventoryItem;

/**
 *
 */
UCLASS()
class GAINX_API UGainXReticleWidget : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    UGainXReticleWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /* Returns the current weapon's diametrical spread angle, in degrees */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float ComputeSpreadAngle() const;

    /* Returns the current weapon's maximum spread radius in screenspace units (pixels) */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    float ComputeMaxScreenspaceSpreadRadius() const;

    /* Returns true if the current weapon is at 'first shot accuracy' */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool HasFirstShotAccuracy() const;

    UFUNCTION(BlueprintCallable)
    void InitializeFromWeapon(UGainXWeaponInstance* InWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void OnWeaponInitialized();

protected:
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UGainXWeaponInstance> WeaponInstance;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UGainXInventoryItem> InventoryItem;
};
