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
    FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
    check(ActorInfo);
    check(InOwnerActor);

    const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

    Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

    if (bHasNewPawnAvatar)
    {
        // Notify all abilities that a new pawn avatar has been set
        for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
        {
            UGainXGameplayAbility* GainXAbilityCDO = CastChecked<UGainXGameplayAbility>(AbilitySpec.Ability);

            if (GainXAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
            {
                TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
                for (UGameplayAbility* AbilityInstance : Instances)
                {
                    UGainXGameplayAbility* GainXAbilityInstance = Cast<UGainXGameplayAbility>(AbilityInstance);
                    if (GainXAbilityInstance)
                    {
                        // Ability instances may be missing for replays
                        GainXAbilityInstance->OnPawnAvatarSet();
                    }
                }
            }
            else
            {
                GainXAbilityCDO->OnPawnAvatarSet();
            }
        }

        // TODO: Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.

        // TODO: initialize GameplayTagPropertyMap in GainXAnimInstance
    }
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

    // Process all abilities that activate when the input is held
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

    // Process all abilities that had their input pressed this frame
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
    {
        if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = true;
                if (AbilitySpec->IsActive())
                {
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

    // Process all abilities that had their input released this frame
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
    {
        if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = false;
                if (AbilitySpec->IsActive())
                {
                    UE_LOG(LogGainXAbilitySystemComponent, Display, TEXT("AbilitySpecInputReleased"));
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
