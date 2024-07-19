// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "GainXGameInstance.generated.h"

UCLASS(Config = Game)
class GAINX_API UGainXGameInstance : public UCommonGameInstance
{
    GENERATED_BODY()

public:
    UGainXGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void Init() override;
    virtual void Shutdown() override;
};
