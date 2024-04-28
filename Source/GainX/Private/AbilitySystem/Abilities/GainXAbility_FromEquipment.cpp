// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbility_FromEquipment.h"
#include "Equipment/GainXEquipmentInstance.h"
#include "Inventory/GainXInventoryItemInstance.h"

UGainXAbility_FromEquipment::UGainXAbility_FromEquipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UGainXEquipmentInstance* UGainXAbility_FromEquipment::GetAssociatedEquipment() const
{
    if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
    {
        return Cast<UGainXEquipmentInstance>(Spec->SourceObject.Get());
    }
    return nullptr;
}

UGainXInventoryItemInstance* UGainXAbility_FromEquipment::GetAssociatedItem() const
{
    if (UGainXEquipmentInstance* Equipment = GetAssociatedEquipment())
    {
        return Cast<UGainXInventoryItemInstance>(Equipment->GetInstigator());
    }
    return nullptr;
}