// GainX, All Rights Reserved

#include "Input/GainXInputComponent.h"

UGainXInputComponent::UGainXInputComponent(const FObjectInitializer& ObjectInitializer) {}

void UGainXInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
    for (uint32 Handle : BindHandles)
    {
        RemoveBindingByHandle(Handle);
    }
    BindHandles.Reset();
}