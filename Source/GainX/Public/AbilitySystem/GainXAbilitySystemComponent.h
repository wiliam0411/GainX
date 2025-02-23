// GainX, All Rights Reserved

#pragma once

#include "AbilitySystemComponent.h"
#include "GainXAbilitySystemComponent.generated.h"

UCLASS()
class GAINX_API UGainXAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UGainXAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /* Updates Input Handles arrays according to the input */
    void AbilityInputTagPressed(const FGameplayTag& InputTag);

    /* Updates Input Handles arrays according to the input */
    void AbilityInputTagReleased(const FGameplayTag& InputTag);

    /* Activates ability according to their activation policy */
    void ProcessAbilityInput();

    /* Clears Input Handles arrays */
    void ClearAbilityInput();

    UFUNCTION(BlueprintCallable)
    bool IsAbilityActive(const FGameplayTagContainer Tags) const;

protected:
    virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
    virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;

    /* Handles to abilities that had their input pressed this frame */
    TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

    /* Handles to abilities that had their input released this frame */
    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

    /* Handles to abilities that have their input held */
    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

    /* Handles to abilities that should be activated on this tick */
    TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

    /* Helper functions for ProcessAbilityInput function */
    void ProcessInputHeld();
    void ProcessInputPressed();
    void ProcessInputReleased();
    void TryActivateAllAbilities();
};