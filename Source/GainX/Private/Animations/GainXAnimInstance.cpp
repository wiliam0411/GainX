// GainX, All Rights Reserved

#include "Animations/GainXAnimInstance.h"
#include "Player/GainXBaseCharacter.h"
#include "Components/GainXCharacterMovementComponent.h"

UGainXAnimInstance::UGainXAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!GetOwningActor()) return;

    AGainXBaseCharacter* Character = Cast<AGainXBaseCharacter>(GetOwningActor());
    if (!Character) return;

    UGainXCharacterMovementComponent* CharMoveComp = Cast<UGainXCharacterMovementComponent>(Character->GetMovementComponent());
    if (!CharMoveComp) return;

    GroundDistance = CharMoveComp->GetGroundDistance();
}
