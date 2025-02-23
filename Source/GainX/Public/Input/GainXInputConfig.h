// GainX, All Rights Reserved

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GainXInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FGainXInputActionEntry
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "Input Action"))
    TObjectPtr<const UInputAction> InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "Input Tag"))
    FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FGainXInputMappingContextEntry
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "Input Mapping Context"))
    TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "Priority"))
    int32 Priority = 0;
};

UCLASS()
class GAINX_API UGainXInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UGainXInputConfig(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "GainX|InputConfig")
    const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;

    UFUNCTION(BlueprintCallable, Category = "GainX|InputConfig")
    const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
    TArray<FGainXInputActionEntry> NativeInputActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
    TArray<FGainXInputActionEntry> AbilityInputActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputMappingContext"))
    TArray<FGainXInputMappingContextEntry> InputMappingContexts;
};