// GainX, All Rights Reserved

#pragma once

#include "AbilitySystemComponent.h"
#include "GainXAbilitySystemComponent.generated.h"

class UAbilityTagRelationshipMapping;

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

    /* Sets the current tag relationship mapping, if null it will clear it out */
    void SetTagRelationshipMapping(UAbilityTagRelationshipMapping* NewMapping) { TagRelationshipMapping = NewMapping; };

protected:
    virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
    virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;

	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;

    UPROPERTY()
    TObjectPtr<UAbilityTagRelationshipMapping> TagRelationshipMapping;

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