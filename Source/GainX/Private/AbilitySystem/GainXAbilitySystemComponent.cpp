// GainX, All Rights Reserved

#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/GainXGameplayAbility.h"
#include "AbilitySystem/AbilityTagRelationshipMapping.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXAbilitySystemComponent, All, All)

UGainXAbilitySystemComponent::UGainXAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;

    ClearAbilityInput();
}

void UGainXAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGainXAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
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

bool UGainXAbilitySystemComponent::IsAbilityActive(const FGameplayTagContainer Tags) const
{
    //ABILITYLIST_SCOPE_LOCK();

    for (const auto Spec : ActivatableAbilities.Items)
    {
        if (!Spec.IsActive() || Spec.Ability == nullptr)
        {
            continue;
        }

        if (Spec.Ability->AbilityTags.HasAny(Tags))
        {
            return true;
        }
    }
    return false;
}

void UGainXAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);
    // We don't support UGameplayAbility::bReplicateInputDirectly.
    // Use replicated events instead so that the WaitInputRelease ability task works.
    if (Spec.IsActive())
    {
        PRAGMA_DISABLE_DEPRECATION_WARNINGS
        const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
        FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
        PRAGMA_ENABLE_DEPRECATION_WARNINGS

        // Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
    }
}

void UGainXAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);
    // We don't support UGameplayAbility::bReplicateInputDirectly.
    // Use replicated events instead so that the WaitInputRelease ability task works.
    if (Spec.IsActive())
    {
        PRAGMA_DISABLE_DEPRECATION_WARNINGS
        const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
        FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
        PRAGMA_ENABLE_DEPRECATION_WARNINGS

        // Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
    }
}

void UGainXAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
    Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

    if (UGainXGameplayAbility* GainXAbility = Cast<UGainXGameplayAbility>(Ability))
    {
        GainXAbility->OnAbilityFailedToActivate(FailureReason);
    }
}

void UGainXAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) 
{
    FGameplayTagContainer ModifiedBlockTags = BlockTags;
    FGameplayTagContainer ModifiedCancelTags = CancelTags;

    if (TagRelationshipMapping)
    {
        TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
    }

    Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags, bExecuteCancelTags, ModifiedCancelTags);
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
