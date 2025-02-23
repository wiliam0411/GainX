// GainX, All Rights Reserved

#include "Input/GainXInputComponent.h"
#include "EnhancedInputSubsystems.h"

UGainXInputComponent::UGainXInputComponent(const FObjectInitializer& ObjectInitializer) {}

void UGainXInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
    for (uint32 Handle : BindHandles)
    {
        RemoveBindingByHandle(Handle);
    }
    BindHandles.Reset();
}

void UGainXInputComponent::AddInputMappingContext(const UGainXInputConfig* InputConfig, APlayerController* PlayerController)
{
    check(InputConfig);
    check(PlayerController);

    if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
    {
        if (auto EnhancedSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            for (const auto InputMappingEntry : InputConfig->InputMappingContexts)
            {
                if (InputMappingEntry.InputMappingContext)
                {
                    EnhancedSubsystem->AddMappingContext(InputMappingEntry.InputMappingContext, InputMappingEntry.Priority);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Trying to add non-valid Input Mapping Context in %s for PlayerController %s"), *InputConfig->GetName(), *PlayerController->GetName());
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("EnhancedInputLocalPlayerSubsystem is not valid for LocalPlayer %s"), *LocalPlayer->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("LocalPlayer is not valid for PlayerController %s"), *PlayerController->GetName());
    }
}
