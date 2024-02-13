// GainX, All Rights Reserved

#include "Input/GainXInputConfig.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"

UGainXInputConfig::UGainXInputConfig(const FObjectInitializer& ObjectInitializer) {}

const UInputAction* UGainXInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
    for (const FGainXInputAction& Action : NativeInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.InputAction;
        }
    }
    UE_LOG(LogTemp, Error, TEXT("Failed find tag"));
    return nullptr;
}