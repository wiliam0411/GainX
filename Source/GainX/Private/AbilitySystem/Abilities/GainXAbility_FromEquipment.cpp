// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbility_FromEquipment.h"
#include "Equipment/GainXEquipmentObject.h"
#include "Inventory/GainXInventoryItemDefinition.h"

UGainXAbility_FromEquipment::UGainXAbility_FromEquipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UGainXEquipmentObject* UGainXAbility_FromEquipment::GetAssociatedEquipment() const
{
    if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
    {
        return Cast<UGainXEquipmentObject>(Spec->SourceObject.Get());
    }
    return nullptr;
}

UGainXInventoryItemDefinition* UGainXAbility_FromEquipment::GetAssociatedItem() const
{
    if (UGainXEquipmentObject* Equipment = GetAssociatedEquipment())
    {
        return Cast<UGainXInventoryItemDefinition>(Equipment->GetInstigator());
    }
    return nullptr;
}