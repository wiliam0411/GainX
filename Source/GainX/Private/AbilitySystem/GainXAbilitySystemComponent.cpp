// GainX, All Rights Reserved

#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/GainXGameplayAbility.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXAbilitySystemComponent, All, All)

UGainXAbilitySystemComponent::UGainXAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ClearAbilityInput();
}

void UGainXAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
    Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UGainXAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;

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
    if (!InputTag.IsValid()) return;

    for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
    {
        if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
        {
            InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
            InputHeldSpecHandles.Remove(AbilitySpec.Handle);
        }
    }
}

void UGainXAbilitySystemComponent::ProcessAbilityInput()
{
    AbilitiesToActivate.Reset();

    ProcessInputHeld();

    ProcessInputPressed();

    TryActivateAllAbilities();

    ProcessInputReleased();

    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
}

void UGainXAbilitySystemComponent::ClearAbilityInput()
{
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
    InputHeldSpecHandles.Reset();
}

void UGainXAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);
    // We don't support UGameplayAbility::bReplicateInputDirectly.
    // Use replicated events instead so that the WaitInputRelease ability task works.
    if (Spec.IsActive())
    {
        // Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
    }
}

void UGainXAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);
    // We don't support UGameplayAbility::bReplicateInputDirectly.
    // Use replicated events instead so that the WaitInputRelease ability task works.
    if (Spec.IsActive())
    {
        // Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
    }
}

void UGainXAbilitySystemComponent::ProcessInputHeld()
{
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
    {
        const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
        if (!AbilitySpec || !AbilitySpec->Ability || AbilitySpec->IsActive())
        {
            continue;
        }

        const UGainXGameplayAbility* GainXAbilityCDO = CastChecked<UGainXGameplayAbility>(AbilitySpec->Ability);
        if (GainXAbilityCDO->GetActivationPolicy() == EGainXAbilityActivationPolicy::WhileInputActive)
        {
            AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
        }
    }
}

void UGainXAbilitySystemComponent::ProcessInputPressed()
{
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
    {
        FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
        if (!AbilitySpec || !AbilitySpec->Ability)
        {
            continue;
        }

        AbilitySpec->InputPressed = true;

        if (AbilitySpec->IsActive())
        {
            AbilitySpecInputPressed(*AbilitySpec);
            continue;
        }

        const UGainXGameplayAbility* GainXAbilityCDO = CastChecked<UGainXGameplayAbility>(AbilitySpec->Ability);
        if (GainXAbilityCDO->GetActivationPolicy() == EGainXAbilityActivationPolicy::OnInputTriggered)
        {
            AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
        }
    }
}

void UGainXAbilitySystemComponent::ProcessInputReleased()
{
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
    {
        FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);

        if (!AbilitySpec || !AbilitySpec->Ability)
        {
            continue;
        }

        AbilitySpec->InputPressed = false;

        if (AbilitySpec->IsActive())
        {
            AbilitySpecInputReleased(*AbilitySpec);
        }
    }
}

void UGainXAbilitySystemComponent::TryActivateAllAbilities() 
{
    for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
    {
        TryActivateAbility(AbilitySpecHandle);
    }
}
