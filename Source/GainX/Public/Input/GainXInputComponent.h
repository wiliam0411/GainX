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

    template <class UserClass, typename FuncType>
    void BindActionByTag(
        const UGainXInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);
};

template <class UserClass, typename FuncType>
void UGainXInputComponent::BindActionByTag(const UGainXInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
    check(InputConfig);
    if (const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag))
    {
        BindAction(IA, TriggerEvent, Object, Func);
    }
}
