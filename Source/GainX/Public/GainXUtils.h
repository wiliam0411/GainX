// GainX, All Rights Reserved

#pragma once

#include "Player/GainXPlayerState.h"

class GainXUtils
{
public:
    template <typename T>
    static T* GetGainXPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    bool static AreEnemies(AController* Controller1, AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

        const auto PlayerState1 = Cast<AGainXPlayerState>(Controller1->PlayerState);
        const auto PlayerState2 = Cast<AGainXPlayerState>(Controller2->PlayerState);

        return false;
    }

    static FText TextFromInt(int32 Number)
    {
        return FText::FromString(FString::FromInt(Number));
    }
};