// GainX, All Rights Reserved

#include "Inventory/GainXInventoryItem.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_Event_Inventory_Stats_Added, "Event.Inventory.Stats.Added");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GainX_Event_Inventory_Stats_Removed, "Event.Inventory.Stats.Removed");

UGainXInventoryItem::UGainXInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

const UGainXInventoryItemFragment* UGainXInventoryItem::FindFragmentByClass(TSubclassOf<UGainXInventoryItemFragment> FragmentClass) const
{
    if (!FragmentClass)
    {
        return nullptr;
    }

    for (UGainXInventoryItemFragment* Fragment : Fragments)
    {
        if (Fragment && Fragment->IsA(FragmentClass))
        {
            return Fragment;
        }
    }

    return nullptr;
}

void UGainXInventoryItem::BroadcastInventoryItemMessage(FGameplayTag Channel, FGameplayTag Stats)
{
    FGainXInventoryItemMessage Message;
    Message.InventoryItem = this;
    Message.InventoryStats = Stats;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSystem.BroadcastMessage(Channel, Message);
}

void UGainXInventoryItem::AddItemStats(FGameplayTag Tag, int32 StackCount)
{
    ItemStats.AddStats(Tag, StackCount);
    OnAddItemStats.Broadcast(Tag);
    BroadcastInventoryItemMessage(TAG_GainX_Event_Inventory_Stats_Added, Tag);
}

void UGainXInventoryItem::RemoveItemStats(FGameplayTag Tag, int32 StackCount)
{
    ItemStats.RemoveStats(Tag, StackCount);
    OnRemoveItemStats.Broadcast(Tag);
    BroadcastInventoryItemMessage(TAG_GainX_Event_Inventory_Stats_Removed, Tag);
}

int32 UGainXInventoryItem::GetItemStatsCount(FGameplayTag Tag) const
{
    return ItemStats.GetStatsCount(Tag);
}

bool UGainXInventoryItem::HasItemStats(FGameplayTag Tag) const
{
    return ItemStats.ContainsTag(Tag);
}
