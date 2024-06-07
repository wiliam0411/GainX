// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/GainXBaseCharacter.h"
#include "GainXAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class GAINX_API AGainXAICharacter : public AGainXBaseCharacter
{
	GENERATED_BODY()
	
public:
	AGainXAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;

	virtual void OnDeath(AActor* OwningActor) override;

	void UpdateHealthWidgetVisibility();
};
