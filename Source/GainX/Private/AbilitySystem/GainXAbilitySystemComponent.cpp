// GainX, All Rights Reserved

#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/GainXGameplayAbility.h"

UGainXAbilitySystemComponent::UGainXAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
    ClearAbilityInput();
}

void UGainXAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        UE_LOG(LogTemp, Display, TEXT("InputTag %s is not valid"), *InputTag.ToString());
        return;
    }
    for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
    {
        if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
        {
            InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
            InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
        }
    }
}

void UGainXAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        UE_LOG(LogTemp, Display, TEXT("InputTag %s is not valid"), *InputTag.ToString());
        return;
    }
    for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
    {
        if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
        {
            InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
            InputHeldSpecHandles.Remove(AbilitySpec.Handle);
        }
    }
}

void UGainXAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
    static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
    AbilitiesToActivate.Reset();

    //UE_LOG(LogTemp, Display, TEXT("InputHeldSpecHandles: %i"), InputHeldSpecHandles.Num());
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
    {
        if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability && !AbilitySpec->IsActive())
            {
                const auto GainXAbilityCDO = CastChecked<UGainXGameplayAbility>(AbilitySpec->Ability);
                if (GainXAbilityCDO->GetActivationPolicy() == EGainXAbilityActivationPolicy::WhileInputActive)
                {
                    AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
                }
            }
        }
    }

    //UE_LOG(LogTemp, Display, TEXT("InputPressedSpecHandles: %i"), InputPressedSpecHandles.Num());
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
    {
        if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = true;

                if (AbilitySpec->IsActive())
                {
                    // Ability is active so pass along the input event.
                    AbilitySpecInputPressed(*AbilitySpec);
                }
                else
                {
                    const auto GainXAbilityCDO = CastChecked<UGainXGameplayAbility>(AbilitySpec->Ability);
                    if (GainXAbilityCDO->GetActivationPolicy() == EGainXAbilityActivationPolicy::OnInputTriggered)
                    {
                        AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
                    }
                }
            }
        }
    }

    for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
    {
        TryActivateAbility(AbilitySpecHandle);
    }

    //UE_LOG(LogTemp, Display, TEXT("InputReleasedSpecHandles: %i"), InputReleasedSpecHandles.Num());
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
    {
        if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = false;
                if (AbilitySpec->IsActive())
                {
                    // Ability is active so pass along the input event.
                    AbilitySpecInputReleased(*AbilitySpec);
                }
            }
        }
    }

    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
}

void UGainXAbilitySystemComponent::ClearAbilityInput()
{
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
    InputHeldSpecHandles.Reset();
}
