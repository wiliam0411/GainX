// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GainXNeedAmmoDecorator.generated.h"

class AGainXBaseWeapon;

UCLASS()
class GAINX_API UGainXNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UGainXNeedAmmoDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<AGainXBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
