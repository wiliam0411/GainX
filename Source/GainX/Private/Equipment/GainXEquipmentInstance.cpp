// GainX, All Rights Reserved

#include "Equipment/GainXEquipmentInstance.h"
#include "Equipment/GainXEquipmentDefinition.h"
#include "GameFramework/Character.h"

UGainXEquipmentInstance::UGainXEquipmentInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

APawn* UGainXEquipmentInstance::GetPawn() const
{
    return Cast<APawn>(GetOuter());
}

void UGainXEquipmentInstance::SpawnEquipmentActors(const TArray<FGainXEquipmentActorToSpawn>& ActorsToSpawn)
{
    APawn* OwningPawn = GetPawn();
    if (!OwningPawn)
    {
        return;
    }

    ACharacter* OwningCharacter = Cast<ACharacter>(OwningPawn);

    USceneComponent* AttachTarget = OwningCharacter ? OwningCharacter->GetMesh() : OwningPawn->GetRootComponent();

    for (const FGainXEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
    {
        AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
        NewActor->FinishSpawning(FTransform::Identity, true);
        NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
        NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
        SpawnedActors.Add(NewActor);
    }
}

void UGainXEquipmentInstance::DestroyEquipmentActors()
{
    for (AActor* Actor : SpawnedActors)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }
}

void UGainXEquipmentInstance::OnEquipped()
{
    K2_OnEquipped();
}

void UGainXEquipmentInstance::OnUnequipped()
{
    K2_OnUnequipped();
}
