// GainX, All Rights Reserved

#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Equipment/GainXEquipmentDefinition.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Equipment/GainXEquipmentObject.h"

/**
 * UGainXEquipmentManagerComponent
 */

UGainXEquipmentManagerComponent::UGainXEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), EquipmentList(this) {}

UGainXEquipmentObject* UGainXEquipmentManagerComponent::EquipItem(TSubclassOf<UGainXEquipmentObject> EquipmentObject)
{
    if (!EquipmentObject)
    {
        return nullptr;
    }

    UGainXEquipmentObject* Result = EquipmentList.AddEntry(EquipmentObject);
    if (Result)
    {
        Result->OnEquipped();
    }

    return Result;
}

void UGainXEquipmentManagerComponent::UnequipItem(UGainXEquipmentObject* EquipmentObject)
{
    if (!EquipmentObject)
    {
        return;
    }

    EquipmentObject->OnUnequipped();
    EquipmentList.RemoveEntry(EquipmentObject);
}

UGainXEquipmentObject* UGainXEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UGainXEquipmentObject> EquipmentObject)
{
    for (FGainXEquipmentEntry& Equipment : EquipmentList.EquipmentEntries)
    {
        UGainXEquipmentObject* Object = Equipment.EquipmentObject;
        if (!Object)
        {
            continue;
        }

        if (Object->IsA(EquipmentObject))
        {
            return Object;
        }
    }
    return nullptr;
}

/**
 * FGainXEquipmentList
 */

UGainXAbilitySystemComponent* FGainXEquipmentList::GetAbilitySystemComponent() const
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    return Cast<UGainXAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UGainXEquipmentObject* FGainXEquipmentList::AddEntry(TSubclassOf<UGainXEquipmentObject> EquipmentObject)
{
    check(EquipmentObject);
    check(OwnerComponent);
    check(OwnerComponent->GetOwner()->HasAuthority());

    UGainXEquipmentObject* Result = nullptr;

    const UGainXEquipmentObject* EquipmentObjectCDO = GetDefault<UGainXEquipmentObject>(EquipmentObject);

    FGainXEquipmentEntry& NewEquipmentEntry = EquipmentEntries.AddDefaulted_GetRef();
    NewEquipmentEntry.EquipmentObject = NewObject<UGainXEquipmentObject>(OwnerComponent->GetOwner(), EquipmentObject);
    Result = NewEquipmentEntry.EquipmentObject;

    if (UGainXAbilitySystemComponent* GainXASC = GetAbilitySystemComponent())
    {
        for (TObjectPtr<const UGainXAbilitySet> AbilitySet : EquipmentObjectCDO->AbilitySetsToGrant)
        {
            AbilitySet->GiveToAbilitySystem(GainXASC, &NewEquipmentEntry.GrantedHandles, Result);
        }
    }

    Result->SpawnEquipmentActors();

    return Result;
}

void FGainXEquipmentList::RemoveEntry(UGainXEquipmentObject* EquipmentObject)
{
    for (auto EntryIt = EquipmentEntries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FGainXEquipmentEntry& Entry = *EntryIt;
        if (Entry.EquipmentObject == EquipmentObject)
        {
            if (UGainXAbilitySystemComponent* GainXASC = GetAbilitySystemComponent())
            {
                Entry.GrantedHandles.TakeFromAbilitySystem(GainXASC);
            }

            EquipmentObject->DestroyEquipmentActors();

            EntryIt.RemoveCurrent();
        }
    }
}
