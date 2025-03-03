// GainX, All Rights Reserved


#include "Pickups/GainXPickupActor.h"

AGainXPickupActor::AGainXPickupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupMesh"));
}