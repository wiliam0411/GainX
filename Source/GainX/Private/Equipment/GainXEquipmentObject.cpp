// GainX, All Rights Reserved

#include "Equipment/GainXEquipmentObject.h"
#include "GameFramework/Character.h"

UGainXEquipmentObject::UGainXEquipmentObject(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

APawn* UGainXEquipmentObject::GetPawn() const
{
    return Cast<APawn>(GetOuter());
}

void UGainXEquipmentObject::OnEquipped()
{
    PlayAnimMontage(EquipMontage);
    K2_OnEquipped();
}

void UGainXEquipmentObject::OnUnequipped()
{
    //PlayAnimMontage(UnequipMontage);
    K2_OnUnequipped();
}

void UGainXEquipmentObject::SpawnEquipmentActors()
{
    APawn* OwningPawn = GetPawn();
    if (!OwningPawn)
    {
        return;
    }

    USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
    if (ACharacter* OwningCharacter = Cast<ACharacter>(OwningPawn))
    {
        AttachTarget = OwningCharacter->GetMesh();
    }

    for (const FGainXEquipmentActorSpawnConfig& SpawnConfig : ActorsToSpawn)
    {
        AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnConfig.Actor, FTransform::Identity, OwningPawn);
        NewActor->FinishSpawning(FTransform::Identity, true);
        NewActor->SetActorRelativeTransform(SpawnConfig.AttachTransform);
        NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnConfig.AttachSocket);

        SpawnedActors.Add(NewActor);
    }
}

void UGainXEquipmentObject::DestroyEquipmentActors()
{
    for (AActor* Actor : SpawnedActors)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }
}

void UGainXEquipmentObject::PlayAnimMontage(UAnimMontage* Montage) 
{
    if (ACharacter* Character = Cast<ACharacter>(GetPawn()))
    {
        Character->PlayAnimMontage(Montage);
    }
}
