// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GainXAttributeSet.h"
#include "GainXHealthSet.generated.h"

UCLASS()
class GAINX_API UGainXHealthSet : public UGainXAttributeSet
{
    GENERATED_BODY()

public:
    UGainXHealthSet();

    ATTRIBUTE_ACCESSORS(UGainXHealthSet, Health);
    ATTRIBUTE_ACCESSORS(UGainXHealthSet, MaxHealth);
    ATTRIBUTE_ACCESSORS(UGainXHealthSet, Healing);
    ATTRIBUTE_ACCESSORS(UGainXHealthSet, Damage);

    /** Delegate when health changes due to damage / healing, some information may be missing on the client */
    mutable FGainXAttributeEvent OnHealthChanged;

    /** Delegate when max health changes */
    mutable FGainXAttributeEvent OnMaxHealthChanged;

    /** Delegate to broadcast when the health attribute reaches zero */
    mutable FGainXAttributeEvent OnOutOfHealth;

protected:
    virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

    void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
    /** Current health attribute */
    UPROPERTY(BlueprintReadOnly, Category = "GainX|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Health;

    /** Current max health attribute */
    UPROPERTY(BlueprintReadOnly, Category = "GainX|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxHealth;

    /** Incoming healing. This is mapped directly to +Health */
    UPROPERTY(BlueprintReadOnly, Category = "GainX|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Healing;

    /** Incoming damage. This is mapped directly to -Health */
    UPROPERTY(BlueprintReadOnly, Category = "GainX|Health", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Damage;

    /** Store Health before any changes */
    float HealthBeforeAttributeChange;

    /** Store MaxHealth before any changes */
    float MaxHealthBeforeAttributeChange;

    /** Used to track when the health reaches 0 */
    bool bOutOfHealth;
};