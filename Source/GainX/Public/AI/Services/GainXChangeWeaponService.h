// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GainXChangeWeaponService.generated.h"

UCLASS()
class GAINX_API UGainXChangeWeaponService : public UBTService
{
	GENERATED_BODY()
	
public:
	UGainXChangeWeaponService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Probability = 0.8f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
