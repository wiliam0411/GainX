// GainX, All Rights Reserved

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GainXInputConfig.generated.h"

class UInputAction;
//struct FGameplayTag;

USTRUCT(BlueprintType)
struct FGainXInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class GAINX_API UGainXInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UGainXInputConfig(const FObjectInitializer& ObjectInitializer);

	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FGainXInputAction> NativeInputActions;
};
