// GainX, All Rights Reserved

#include "Interaction/GainXWorldCollectable.h"

AGainXWorldCollectable::AGainXWorldCollectable() {}

void AGainXWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) 
{
    OptionBuilder.AddInteractionOption(Option);
}

TSubclassOf<UGameplayAbility> AGainXWorldCollectable::GetInteractionAbility() const
{
    return InteractionAbilityToGrant;
}

