// GainX, All Rights Reserved

#include "Animations/GainXAnimNotify.h"

void UGainXAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}