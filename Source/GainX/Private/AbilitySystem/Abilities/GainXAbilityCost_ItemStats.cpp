// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbilityCost_ItemStats.h"
#include "AbilitySystem/Abilities/GainXAbility_FromEquipment.h"
#include "Inventory/GainXInventoryItemDefinition.h"

UGainXAbilityCost_ItemStats::UGainXAbilityCost_ItemStats() {}

bool UGainXAbilityCost_ItemStats::CheckCost(
    const UGainXGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
    const auto EquipmentAbility = Cast<const UGainXAbility_FromEquipment>(Ability);
    if (!EquipmentAbility)
    {
        return false;
    }

    UGainXInventoryItemDefinition* InventoryItem = EquipmentAbility->GetAssociatedItem();
    if (!InventoryItem)
    {
        return false;
    }

    const bool bCanApplyCost = InventoryItem->GetItemStatsCount(Tag) >= Quantity;

    if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
    {
        OptionalRelevantTags->AddTag(FailureTag);
    }

    return bCanApplyCost;
}

void UGainXAbilityCost_ItemStats::ApplyCost(
    const UGainXGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    const auto EquipmentAbility = Cast<const UGainXAbility_FromEquipment>(Ability);
    if (!EquipmentAbility)
    {
        return;
    }

    UGainXInventoryItemDefinition* InventoryItem = EquipmentAbility->GetAssociatedItem();
    if (!InventoryItem)
    {
        return;
    }

    InventoryItem->RemoveItemStats(Tag, Quantity);
}