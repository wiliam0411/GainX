// GainX, All Rights Reserved

#include "Equipment/GainXEquipmentManagerComponent.h"
#include "AbilitySystem/GainXAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"

#include "AbilitySystemGlobals.h"
#include "Equipment/GainXEquipmentActor.h"
#include "Engine/World.h"
#include "Player/GainXBaseCharacter.h"
#include "NativeGameplayTags.h"

DEFINE_LOG_CATEGORY_STATIC(LogGainXEquipmentManagerComponent, All, All)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_Initialization_EquipmentComponent, "Event.Initialization.EquipmentComponent");

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_Event_Equipment_Equip, "Event.Equipment.Equip");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_Event_Equipment_Unequip, "Event.Equipment.Unequip");

UGainXEquipmentManagerComponent::UGainXEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), EquipmentList(this) {}

void UGainXEquipmentManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}

AGainXEquipmentActor* UGainXEquipmentManagerComponent::EquipItem(TSubclassOf<AGainXEquipmentActor> EquipmentItemClass, UGainXInventoryItem* AssociatedInventoryItem)
{
    if (!EquipmentItemClass)
    {
        return nullptr;
    }

    if (AGainXEquipmentActor* NewEquipmentItem = EquipmentList.AddEntry(EquipmentItemClass))
    {
        NewEquipmentItem->SetAssociatedInventoryItem(AssociatedInventoryItem);

        NewEquipmentItem->OnEquipped();

        OnEquipItem.Broadcast(NewEquipmentItem);

        BroadcastEquipmentMessage(TAG_GainX_Event_Equipment_Equip, NewEquipmentItem);

        return NewEquipmentItem;
    }

    UE_LOG(LogGainXEquipmentManagerComponent, Error, TEXT("EquipItem: Failed to add new equipment item for class: %s"), *EquipmentItemClass->GetName());

    return nullptr;
}

void UGainXEquipmentManagerComponent::UnequipItem(AGainXEquipmentActor* EquipmentItem)
{
    if (!EquipmentItem)
    {
        return;
    }

    EquipmentItem->OnUnequipped();

    OnUnequipItem.Broadcast();

    BroadcastEquipmentMessage(TAG_GainX_Event_Equipment_Unequip, EquipmentItem);

    EquipmentList.RemoveEntry(EquipmentItem);
}

AGainXEquipmentActor* UGainXEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<AGainXEquipmentActor> EquipmentActorClass)
{
    if (!EquipmentActorClass)
    {
        UE_LOG(LogGainXEquipmentManagerComponent, Warning, TEXT("GetFirstInstanceOfType failed: EquipmentObjectClass is null."));

        return nullptr;
    }

    for (FGainXEquipmentEntry& EquipmentEntry : EquipmentList.EquipmentEntries)
    {
        if (EquipmentEntry.Item && EquipmentEntry.Item->IsA(EquipmentActorClass))
        {
            return EquipmentEntry.Item;
        }
    }

    UE_LOG(LogGainXEquipmentManagerComponent, Warning, TEXT("GetFirstInstanceOfType: No instance of type %s found."), *EquipmentActorClass->GetName());

    return nullptr;
}

void UGainXEquipmentManagerComponent::BroadcastEquipmentMessage(FGameplayTag Channel, AGainXEquipmentActor* EquipmentActor)
{
    FGainXEquipmentMessage Message;
    Message.EquipmentManagerComponent = this;
    Message.EquipmentActor = EquipmentActor;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetOwner()->GetWorld());
    MessageSystem.BroadcastMessage(Channel, Message);
}

UGainXAbilitySystemComponent* FGainXEquipmentList::GetAbilitySystemComponent() const
{
    check(EquipmentManager);
    AActor* OwningActor = EquipmentManager->GetOwner();
    return Cast<UGainXAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

AGainXEquipmentActor* FGainXEquipmentList::AddEntry(TSubclassOf<AGainXEquipmentActor> EquipmentItemClass)
{
    check(EquipmentItemClass);
    check(EquipmentManager);
    check(EquipmentManager->GetOwner()->HasAuthority());

    FGainXEquipmentEntry& NewEquipmentEntry = EquipmentEntries.AddDefaulted_GetRef();

    if (AGainXBaseCharacter* OwningCharacter = Cast<AGainXBaseCharacter>(EquipmentManager->GetOwner()))
    {
        if (USkeletalMeshComponent* FirstPersonMesh = OwningCharacter->GetFirstPersonMesh())
        {
            AGainXEquipmentActor* NewActor = EquipmentManager->GetWorld()->SpawnActorDeferred<AGainXEquipmentActor>(EquipmentItemClass, FTransform::Identity, OwningCharacter);
            NewActor->FinishSpawning(FTransform::Identity, true);
            NewActor->SetActorRelativeTransform(NewActor->AttachTransform);
            NewActor->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, NewActor->AttachSocket);
            NewEquipmentEntry.Item = NewActor;
        }
    }

    // If the Ability System Component is valid, grant item's abilities
    if (UGainXAbilitySystemComponent* GainXASC = GetAbilitySystemComponent())
    {
        const AGainXEquipmentActor* EquipmentObjectCDO = GetDefault<AGainXEquipmentActor>(EquipmentItemClass);
        for (TObjectPtr<const UGainXAbilitySet> AbilitySet : EquipmentObjectCDO->AbilitySetsToGrant)
        {
            if (AbilitySet)
            {
                AbilitySet->GiveToAbilitySystem(GainXASC, &NewEquipmentEntry.GrantedHandles, NewEquipmentEntry.Item);
            }
        }
    }
    return NewEquipmentEntry.Item;
}

void FGainXEquipmentList::RemoveEntry(AGainXEquipmentActor* EquipmentItem)
{
    check(EquipmentItem);

    // Iterate through the EquipmentEntries to find the matching EquipmentItem
    for (auto EquipmentEntryIt = EquipmentEntries.CreateIterator(); EquipmentEntryIt; ++EquipmentEntryIt)
    {
        FGainXEquipmentEntry& EquipmentEntry = *EquipmentEntryIt;
        if (EquipmentEntry.Item == EquipmentItem)
        {
            // Remove granted item's abilities from the ability system component
            if (UGainXAbilitySystemComponent* GainXASC = GetAbilitySystemComponent())
            {
                EquipmentEntry.GrantedHandles.TakeFromAbilitySystem(GainXASC);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Ability System Component is null. Cannot remove granted handles for item: %s"), *EquipmentItem->GetName());
            }

            EquipmentItem->Destroy();

            EquipmentEntryIt.RemoveCurrent();
        }
    }
}
