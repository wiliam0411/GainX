// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Input/GainXInputConfig.h"
#include "GainXInputComponent.generated.h"

UCLASS(Config = Input)
class GAINX_API UGainXInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

public:
    UGainXInputComponent(const FObjectInitializer& ObjectInitializer);

    /* Binds function to single native input action */
    template <class UserClass, typename FuncType>
    void BindNativeActionByTag(const UGainXInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

    /* Binds pressed and released functions to input action and passes them input tag as a parameter */
    template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
    void BindAbilityActions(const UGainXInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

    /* Removes the binds with the specified handles */
    void RemoveBinds(TArray<uint32>& BindHandles);

    /* Adds mapping context for given controller */
    void AddInputMappingContext(const UGainXInputConfig* InputConfig, APlayerController* PlayerController);
};

template <class UserClass, typename FuncType>
void UGainXInputComponent::BindNativeActionByTag(const UGainXInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
    check(InputConfig);

    if (const UInputAction* InputAction = InputConfig->FindNativeInputActionForTag(InputTag))
    {
        BindAction(InputAction, TriggerEvent, Object, Func);
    }
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGainXInputComponent::BindAbilityActions(const UGainXInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
    check(InputConfig);

    for (const FGainXInputActionEntry& InputActionEntry : InputConfig->AbilityInputActions)
    {
        if (InputActionEntry.InputAction && InputActionEntry.InputTag.IsValid())
        {
            if (PressedFunc)
            {
                BindHandles.Add(BindAction(InputActionEntry.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputActionEntry.InputTag).GetHandle());
            }

            if (ReleasedFunc)
            {
                BindHandles.Add(BindAction(InputActionEntry.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputActionEntry.InputTag).GetHandle());
            }
        }
    }
}
