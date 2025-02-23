// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GainXGameplayAbility.generated.h"

class AGainXPlayerCharacter;
class AGainXPlayerController;
class UGainXAbilitySystemComponent;
class UGainXAbilityCost;

UENUM(BlueprintType)
enum class EGainXAbilityActivationPolicy : uint8
{
    /* Try to activate the ability when the input is triggered. */
    OnInputTriggered,

    /* Continually try to activate the ability while the input is active. */
    WhileInputActive,

    /* Try to activate the ability when an avatar is assigned. */
    OnSpawn
};

UCLASS(Blueprintable, BlueprintType)
class GAINX_API UGainXGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()
    friend class UGainXAbilitySystemComponent;

public:
    UGainXGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    UGainXAbilitySystemComponent* GetGainXAbilitySystemComponentFromActorInfo() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    AGainXPlayerController* GetGainXPlayerControllerFromActorInfo() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    AGainXPlayerCharacter* GetGainXCharacterFromActorInfo() const;

    EGainXAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

    void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

    void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason);

protected:
    //~UGameplayAbility interface
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    //~End of UGameplayAbility interface

    /* Called when this ability is granted to the ability system component. */
    UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
    void K2_OnAbilityAdded();

    /* Called when this ability is removed from the ability system component. */
    UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
    void K2_OnAbilityRemoved();

    UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityFailedToActivate")
    void K2_OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason);

protected:
    /* Defines how this ability is meant to activate */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Activation)
    EGainXAbilityActivationPolicy ActivationPolicy;
};