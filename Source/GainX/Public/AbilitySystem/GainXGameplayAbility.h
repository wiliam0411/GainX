// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GainXGameplayAbility.generated.h"

class AGainXBaseCharacter;
class UGainXAbilitySystemComponent;

UENUM(BlueprintType)
enum class EGainXAbilityActivationPolicy : uint8
{
    // Try to activate the ability when the input is triggered.
    OnInputTriggered,

    // Continually try to activate the ability while the input is active.
    WhileInputActive,

    // Try to activate the ability when an avatar is assigned.
    OnSpawn
};

UCLASS()
class GAINX_API UGainXGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()
    //friend class UGainXAbilitySystemComponent;

public:
    UGainXGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponentFromActorInfo() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    AGainXBaseCharacter* GetGainXCharacterFromActorInfo() const;

    EGainXAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GainX|Ability")
    EGainXAbilityActivationPolicy ActivationPolicy;
};