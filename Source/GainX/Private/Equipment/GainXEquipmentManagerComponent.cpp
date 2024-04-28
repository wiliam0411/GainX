// GainX, All Rights Reserved

#include "Equipment/GainXEquipmentManagerComponent.h"
#include "Equipment/GainXEquipmentDefinition.h"
#include "Equipment/GainXEquipmentInstance.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "AbilitySystem/GainXAbilitySet.h"
#include "AbilitySystemGlobals.h"

UGainXEquipmentManagerComponent::UGainXEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), EquipmentList(this) {}

UGainXEquipmentInstance* UGainXEquipmentManagerComponent::EquipItem(TSubclassOf<UGainXEquipmentDefinition> EquipmentDef)
{
    UGainXEquipmentInstance* Result = nullptr;
    if (EquipmentDef != nullptr)
    {
        Result = EquipmentList.AddEntry(EquipmentDef);
        if (Result != nullptr)
        {
            Result->OnEquipped();
        }
    }
    return Result;
}

void UGainXEquipmentManagerComponent::UnequipItem(UGainXEquipmentInstance* ItemInstance)
{
    if (ItemInstance != nullptr)
    {
        ItemInstance->OnUnequipped();
        EquipmentList.RemoveEntry(ItemInstance);
    }
}

UGainXEquipmentInstance* UGainXEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UGainXEquipmentInstance> InstanceType)
{
    for (FGainXEquipmentEntry& Entry : EquipmentList.Entries)
    {
        if (UGainXEquipmentInstance* Instance = Entry.Instance)
        {
            if (Instance->IsA(InstanceType))
            {
                return Instance;
            }
        }
    }

    return nullptr;
}

UGainXEquipmentInstance* FGainXEquipmentList::AddEntry(TSubclassOf<UGainXEquipmentDefinition> EquipmentDef)
{
    UGainXEquipmentInstance* Result = nullptr;

    check(EquipmentDef != nullptr);
    check(OwnerComponent);
    check(OwnerComponent->GetOwner()->HasAuthority());

    const UGainXEquipmentDefinition* EquipmentCDO = GetDefault<UGainXEquipmentDefinition>(EquipmentDef);

    TSubclassOf<UGainXEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
    if (InstanceType == nullptr)
    {
        InstanceType = UGainXEquipmentInstance::StaticClass();
    }

    FGainXEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.EquipmentDefinition = EquipmentDef;
    NewEntry.Instance = NewObject<UGainXEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
    Result = NewEntry.Instance;

    if (UGainXAbilitySystemComponent* ASC = GetAbilitySystemComponent())
    {
        for (TObjectPtr<const UGainXAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
        {
            AbilitySet->GiveToAbilitySystem(ASC, Result);
        }
    }

    Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

    return Result;
}

void FGainXEquipmentList::RemoveEntry(UGainXEquipmentInstance* Instance)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FGainXEquipmentEntry& Entry = *EntryIt;
        if (Entry.Instance == Instance)
        {
            if (UGainXAbilitySystemComponent* ASC = GetAbilitySystemComponent())
            {
                // TODO: Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
            }

            Instance->DestroyEquipmentActors();

            EntryIt.RemoveCurrent();
        }
    }
}

UGainXAbilitySystemComponent* FGainXEquipmentList::GetAbilitySystemComponent() const
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    return Cast<UGainXAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}
