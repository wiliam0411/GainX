// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ScanInteractablesNearby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScanInteractablesNearbyDelegate, const TArray<AActor*>&, OverlapActors);

/**
 *
 */
UCLASS()
class GAINX_API UScanInteractablesNearby : public UAbilityTask
{
    GENERATED_BODY()

public:
    UScanInteractablesNearby(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UScanInteractablesNearby* ScanInteractablesNearby(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate, ECollisionChannel CollisionChannel);

    //~ Begin UAbilityTask interface
    virtual void Activate() override;
    virtual void OnDestroy(bool AbilityEnded) override;
    //~ End UAbilityTask interface

public:
    UPROPERTY(BlueprintAssignable)
    FScanInteractablesNearbyDelegate OnTaskUpdated;

private:
    void PerformScan();

private:
    float ScanRange = 100.0f;
    float ScanRate = 0.1f;
    ECollisionChannel CollisionChannel;

    FTimerHandle ScanTimerHandle;
};
