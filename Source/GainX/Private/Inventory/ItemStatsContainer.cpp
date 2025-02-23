// GainX, All Rights Reserved

#include "Inventory/ItemStatsContainer.h"

DEFINE_LOG_CATEGORY_STATIC(LogItemStatsContainer, All, All)

void FItemStatsContainer::AddStats(FGameplayTag Tag, int32 StackCount)
{
    if (!Tag.IsValid())
    {
        UE_LOG(LogItemStatsContainer, Warning, TEXT("AddStats: Invalid stats tag"));
        return;
    }

    if (StackCount < 1)
    {
        UE_LOG(LogItemStatsContainer, Warning, TEXT("AddStats: Invalid stack count (%d). Must be at least 1"), StackCount);
        return;
    }

    // Iterate through the StatsContainer to find the matching Tag
    for (FItemStats& Stats : StatsContainer)
    {
        if (Stats.Tag == Tag)
        {
            Stats.StackCount += StackCount;
            return;
        }
    }

    // If no matching Tag was found, add a new FItemStats entry
    StatsContainer.Add(FItemStats(Tag, StackCount));
}

void FItemStatsContainer::RemoveStats(FGameplayTag Tag, int32 StackCount)
{
    if (!Tag.IsValid())
    {
        UE_LOG(LogItemStatsContainer, Warning, TEXT("RemoveStats: Invalid Tag"));
        return;
    }

    if (StackCount < 1)
    {
        UE_LOG(LogItemStatsContainer, Warning, TEXT("RemoveStats: Invalid stack count (%d). Must be at least 1"), StackCount);
        return;
    }

    // Iterate through the StatsContainer to find the matching Tag
    for (auto It = StatsContainer.CreateIterator(); It; ++It)
    {
        FItemStats& Stats = *It;
        if (Stats.Tag == Tag)
        {
            // If StackCount to remove is greater than or equal to current StackCount, remove the entry
            if (Stats.StackCount <= StackCount)
            {
                It.RemoveCurrent();
            }

            // Otherwise, decrease the StackCount
            else
            {
                Stats.StackCount -= StackCount;
            }

            return;
        }
    }

    UE_LOG(LogItemStatsContainer, Warning, TEXT("RemoveStats: Tag %s not found in StatsContainer"), *Tag.ToString());
}

int32 FItemStatsContainer::GetStatsCount(FGameplayTag Tag) const
{
    for (const FItemStats& Stats : StatsContainer)
    {
        if (Stats.Tag == Tag)
        {
            return Stats.StackCount;
        }
    }
    return 0;
}

bool FItemStatsContainer::ContainsTag(FGameplayTag Tag) const
{
    for (const FItemStats& Stats : StatsContainer)
    {
        if (Stats.Tag == Tag)
        {
            return true;
        }
    }
    return false;
}
