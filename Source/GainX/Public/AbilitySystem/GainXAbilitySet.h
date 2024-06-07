// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "GainXAbilitySet.generated.h"

class UGainXGameplayAbility;
class UGainXAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

/**
 * FGainXAbilitySet_GameplayAbility
 *
 * Data used by the ability set to grant gameplay abilities.
 */
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

/**
 * FGainXAbilitySet_AttributeSet
 *
 * Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FGainXAbilitySet_AttributeSet
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UAttributeSet> AttributeSet;
};

/**
 * FGainXAbilitySet_GameplayEffect
 *
 * Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FGainXAbilitySet_GameplayEffect
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

    UPROPERTY(EditDefaultsOnly)
    float EffectLevel = 1.0f;
};

/**
 * FGainXAbilitySet_GrantedHandles
 *
 * Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FGainXAbilitySet_GrantedHandles
{
    GENERATED_BODY()

public:
    void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
    void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
    void AddAttributeSet(UAttributeSet* Set);

    void TakeFromAbilitySystem(UGainXAbilitySystemComponent* LyraASC);

protected:
    // Handles to the granted abilities.
    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

    // Handles to the granted gameplay effects.
    UPROPERTY()
    TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

    // Pointers to the granted attribute sets
    UPROPERTY()
    TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 * UGainXAbilitySet
 *
 * Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS(BlueprintType, Const)
class GAINX_API UGainXAbilitySet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UGainXAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void GiveToAbilitySystem(UGainXAbilitySystemComponent* GainXASC, FGainXAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
    // Gameplay abilities to grant when this ability set is granted.
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
    TArray<FGainXAbilitySet_GameplayAbility> GrantedGameplayAbilities;

    // Attribute sets to grant when this ability set is granted.
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Attribute Sets", meta = (TitleProperty = AttributeSet))
    TArray<FGainXAbilitySet_AttributeSet> GrantedGameplayAttributeSets;

    // Gameplay effects to grant when this ability set is granted.
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
    TArray<FGainXAbilitySet_GameplayEffect> GrantedGameplayEffects;
};
