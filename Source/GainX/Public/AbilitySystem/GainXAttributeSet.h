// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GainXAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                       \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                             \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GAINX_API UGainXAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UGainXAttributeSet();

    ATTRIBUTE_ACCESSORS(UGainXAttributeSet, Health);

    UPROPERTY(BlueprintReadOnly, Category = "GainX|Health")
    FGameplayAttributeData Health;
};
