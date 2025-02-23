// GainX, All Rights Reserved


#include "Equipment/GainXEquipmentActor.h"

// Sets default values
AGainXEquipmentActor::AGainXEquipmentActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGainXEquipmentActor::OnEquipped() 
{
    K2_OnEquipped();
}

void AGainXEquipmentActor::OnUnequipped() 
{
    K2_OnUnequipped();
}

APawn* AGainXEquipmentActor::GetPawn() const
{
    return Cast<APawn>(GetOwner());
}

UAnimMontage* AGainXEquipmentActor::GetAnimMontageByTag(FGameplayTag Tag) const
{
    if (AnimSet.Contains(Tag))
    {
        if (const auto Anim = *AnimSet.Find(Tag))
        {
            return Cast<UAnimMontage>(Anim);
        }
    }

    return nullptr;
}

UAnimSequence* AGainXEquipmentActor::GetAnimSequenceByTag(FGameplayTag Tag) const
{
    if (AnimSet.Contains(Tag))
    {
        if (const auto Anim = *AnimSet.Find(Tag))
        {
            return Cast<UAnimSequence>(Anim);
        }
    }
    
    return nullptr;
}