// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbility_FromEquipment.h"
#include "Inventory/GainXInventoryItem.h"
#include "Equipment/GainXEquipmentActor.h"

UGainXAbility_FromEquipment::UGainXAbility_FromEquipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

AGainXEquipmentActor* UGainXAbility_FromEquipment::GetAssociatedEquipmentActor(TSubclassOf<AGainXEquipmentActor> EquipmentActorClass) const
{
    if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
    {
        if (Spec->SourceObject.Get() && Spec->SourceObject.Get()->IsA(EquipmentActorClass))
        {
            return Cast<AGainXEquipmentActor>(Spec->SourceObject.Get());
        }
    }

    return nullptr;
}

UGainXInventoryItem* UGainXAbility_FromEquipment::GetAssociatedInventoryItem() const
{
    if (AGainXEquipmentActor* Equipment = GetAssociatedEquipmentActor(AGainXEquipmentActor::StaticClass()))
    {
        return Equipment->GetAssociatedInventoryItem();
    }

    return nullptr;
}