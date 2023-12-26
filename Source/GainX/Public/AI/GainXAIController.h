// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GainXAIController.generated.h"

class UGainXAIPerceptionComponent;
class UGainXRespawnComponent;

UCLASS()
class GAINX_API AGainXAIController : public AAIController
{
    GENERATED_BODY()

public:
    AGainXAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    UGainXAIPerceptionComponent* GainXAIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componenets")
    UGainXRespawnComponent* RespawnComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* GetFocusOnActor() const;
};
