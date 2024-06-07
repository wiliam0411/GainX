// GainX, All Rights Reserved

#include "Inventory/ItemStatsContainer.h"

void FItemStatsContainer::AddStats(FGameplayTag Tag, int32 StackCount)
{
    if (!Tag.IsValid())
    {
        return;
    }

    if (StackCount < 1)
    {
        return;
    }

    for (FItemStats& Stats : StatsContainer)
    {
        if (Stats.Tag == Tag)
        {
            Stats.StackCount += StackCount;
            return;
        }
    }

    StatsContainer.Add(FItemStats(Tag, StackCount));
}

void FItemStatsContainer::RemoveStats(FGameplayTag Tag, int32 StackCount)
{
    if (!Tag.IsValid())
    {
        return;
    }

    if (StackCount < 1)
    {
        return;
    }

    for (auto It = StatsContainer.CreateIterator(); It; ++It)
    {
        FItemStats& Stats = *It;
        if (Stats.Tag == Tag)
        {
            if (Stats.StackCount <= StackCount)
            {
                It.RemoveCurrent();
            }
            else
            {
                Stats.StackCount -= StackCount;
            }
            return;
        }
    }
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
