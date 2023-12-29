// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GainXAssetManager.generated.h"

UCLASS()
class GAINX_API UGainXAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    UGainXAssetManager();

    static UGainXAssetManager& Get();
};