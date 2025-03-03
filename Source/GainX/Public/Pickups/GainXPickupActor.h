// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GainXPickupActor.generated.h"

UCLASS()
class GAINX_API AGainXPickupActor : public AActor
{
    GENERATED_BODY()

public:
    AGainXPickupActor();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* PickupMesh;
};
