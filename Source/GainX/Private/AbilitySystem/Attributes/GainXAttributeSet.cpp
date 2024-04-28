// GainX, All Rights Reserved

#include "AbilitySystem/Attributes/GainXAttributeSet.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"

UGainXAttributeSet::UGainXAttributeSet() {}

UGainXAbilitySystemComponent* UGainXAttributeSet::GetGainXAbilitySystemComponent() const
{
    return Cast<UGainXAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}