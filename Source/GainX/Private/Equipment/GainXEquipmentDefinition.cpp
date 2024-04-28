// GainX, All Rights Reserved

#include "Equipment/GainXEquipmentDefinition.h"
#include "Equipment/GainXEquipmentInstance.h"
#include "AbilitySystem/GainXAbilitySet.h"

UGainXEquipmentDefinition::UGainXEquipmentDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    InstanceType = UGainXEquipmentInstance::StaticClass();
}