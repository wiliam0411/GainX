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

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);

    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
    void ClearAbilityInput();

protected:
    virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
    virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

    /** Handles to abilities that had their input pressed this frame */
    TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

    /** Handles to abilities that had their input released this frame */
    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

    /** Handles to abilities that have their input held */
    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};