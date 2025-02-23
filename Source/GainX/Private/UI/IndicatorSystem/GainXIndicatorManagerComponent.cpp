// GainX, All Rights Reserved

#include "UI/IndicatorSystem/GainXIndicatorManagerComponent.h"
#include "UI/IndicatorSystem/IndicatorDescriptor.h"

UGainXIndicatorManagerComponent::UGainXIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bAutoRegister = true;
    bAutoActivate = true;
}

UGainXIndicatorManagerComponent* UGainXIndicatorManagerComponent::GetComponent(AController* Controller)
{
    if (Controller)
    {
        return Controller->FindComponentByClass<UGainXIndicatorManagerComponent>();
    }

    return nullptr;
}

void UGainXIndicatorManagerComponent::AddIndicator(UIndicatorDescriptor* IndicatorDescriptor) 
{
    IndicatorDescriptor->SetIndicatorManagerComponent(this);
    OnIndicatorAdded.Broadcast(IndicatorDescriptor);
    Indicators.Add(IndicatorDescriptor);
}

void UGainXIndicatorManagerComponent::RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor) 
{
    if (IndicatorDescriptor)
    {
        ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);

        OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
        Indicators.Remove(IndicatorDescriptor);
    }
}