// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GainXAttributeSet.generated.h"

class UGainXAbilitySystemComponent;
struct FGameplayEffectSpec;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                                                                                   \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                                                                                         \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Delegate used to broadcast attribute events:
 * @param EffectInstigator	The original instigating actor for this event
 * @param EffectCauser		The physical actor that caused the change
 * @param EffectSpec		The full effect spec for this change
 * @param EffectMagnitude	The raw magnitude, this is before clamping
 * @param OldValue			The value of the attribute before it was changed
 * @param NewValue			The value after it was changed
 */
DECLARE_MULTICAST_DELEGATE_SixParams(
    FGainXAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

UCLASS()
class GAINX_API UGainXAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UGainXAttributeSet();

    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponent() const;
};