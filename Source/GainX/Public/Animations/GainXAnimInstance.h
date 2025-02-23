// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "GainXAnimInstance.generated.h"

/**
 * The base game animation instance class used by this project.
 */
UCLASS()
class GAINX_API UGainXAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UGainXAnimInstance(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterStateData")
    float GroundDistance = -1.0f;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
    FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

private:
    void UpdateGroundDistance();
};