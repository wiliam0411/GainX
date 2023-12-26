// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GainXHealthPercentDecorator.generated.h"

UCLASS()
class GAINX_API UGainXHealthPercentDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UGainXHealthPercentDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.6f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};