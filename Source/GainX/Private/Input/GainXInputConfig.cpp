// GainX, All Rights Reserved

#include "Input/GainXInputConfig.h"
#include "InputAction.h"

UGainXInputConfig::UGainXInputConfig(const FObjectInitializer& ObjectInitializer) {}

const UInputAction* UGainXInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
    for (const FGainXInputAction& Action : NativeInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.InputAction;
        }
    }
    UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
    return nullptr;
}

const UInputAction* UGainXInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
    for (const FGainXInputAction& Action : AbilityInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.InputAction;
        }
    }
    UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));

    return nullptr;
}