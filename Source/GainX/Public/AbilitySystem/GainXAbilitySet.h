// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GainXAbilitySet.generated.h"

class UGainXGameplayAbility;
class UGainXAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FGainXAbilitySet_GameplayAbility
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGainXGameplayAbility> Ability = nullptr;

    UPROPERTY(EditDefaultsOnly)
    int32 AbilityLevel = 1;

    UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
    FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class GAINX_API UGainXAbilitySet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UGainXAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void GiveToAbilitySystem(UGainXAbilitySystemComponent* GainXASC) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
    TArray<FGainXAbilitySet_GameplayAbility> GameplayAbilities;
};
