// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractableTarget.h"
#include "Interaction/InteractionOption.h"
#include "GainXWorldCollectable.generated.h"

UCLASS()
class GAINX_API AGainXWorldCollectable : public AActor, public IInteractableTarget
{
    GENERATED_BODY()

public:
    AGainXWorldCollectable();

    //~IInteractableTarget interface
    virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
    virtual TSubclassOf<UGameplayAbility> GetInteractionAbility() const override;
    //~End of IInteractableTarget interface

protected:
    UPROPERTY(EditAnywhere)
    FInteractionOption Option;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;
};
