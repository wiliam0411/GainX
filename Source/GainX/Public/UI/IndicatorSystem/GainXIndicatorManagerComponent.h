// GainX, All Rights Reserved

#pragma once

#include "Components/ControllerComponent.h"

#include "GainXIndicatorManagerComponent.generated.h"

class UIndicatorDescriptor;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class GAINX_API UGainXIndicatorManagerComponent : public UControllerComponent
{
    GENERATED_BODY()

public:
    UGainXIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

    static UGainXIndicatorManagerComponent* GetComponent(AController* Controller);

    UFUNCTION(BlueprintCallable, Category = Indicator)
    void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);

    UFUNCTION(BlueprintCallable, Category = Indicator)
    void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

    DECLARE_EVENT_OneParam(UGainXIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor) 
    FIndicatorEvent OnIndicatorAdded;
    FIndicatorEvent OnIndicatorRemoved;

    const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }
private:

    UPROPERTY()
    TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
};
