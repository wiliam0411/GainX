// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GainXAbilitySystemComponent.generated.h"

UCLASS()
class GAINX_API UGainXAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UGainXAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Notifies abilities about new pawn and initialize GameplayTagPropertyMap */
    virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

    /** Updates Input Handles arrays according to the input */
    void AbilityInputTagPressed(const FGameplayTag& InputTag);

    /** Updates Input Handles arrays according to the input */
    void AbilityInputTagReleased(const FGameplayTag& InputTag);

    /** Activates ability according to their activation policy */
    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

    /** Clears Input Handles arrays */
    void ClearAbilityInput();

protected:
    /** Wrap function that invokes input event for Wait Input Release Node */
    virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;

    /** Wrap function that invokes input event for Wait Input Release Node */
    virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

    /** Handles to abilities that had their input pressed this frame */
    TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

    /** Handles to abilities that had their input released this frame */
    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

    /** Handles to abilities that have their input held */
    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

private:
    /** Handles to abilities that should be activated on this tick */
    TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

    /** Helper functions for ProcessAbilityInput function */
    void ProcessInputHeld();
    void ProcessInputPressed();
    void ProcessInputReleased();
    void TryActivateAllAbilities();
};