// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ItemStatsContainer.generated.h"

USTRUCT(BlueprintType)
struct FItemStats
{
    GENERATED_BODY()

public:
    FItemStats() {}

    FItemStats(FGameplayTag InTag, int32 InStackCount) : Tag(InTag), StackCount(InStackCount) {}

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag Tag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 StackCount = 0;
};

USTRUCT(BlueprintType)
struct FItemStatsContainer
{
    GENERATED_BODY()

public:
    FItemStatsContainer() {}

    /** Adds a specified number of stacks to the tag (does nothing if StackCount is below 1) */
    void AddStats(FGameplayTag Tag, int32 StackCount);

    /** Removes a specified number of stacks from the tag (does nothing if StackCount is below 1) */
    void RemoveStats(FGameplayTag Tag, int32 StackCount);

    /** Returns the stack count of the specified tag (or 0 if the tag is not present) */
    int32 GetStatsCount(FGameplayTag Tag) const;

    /** Returns true if there is at least one stack of the specified tag */
    bool ContainsTag(FGameplayTag Tag) const;

private:
    UPROPERTY()
    TArray<FItemStats> StatsContainer;
};
