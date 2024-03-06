// GainX, All Rights Reserved

#include "Animations/GainXAnimInstance.h"
#include "Player/GainXBaseCharacter.h"
#include "Components/GainXCharacterMovementComponent.h"
#include "Components/GainXWeaponComponent.h"
#include "Weapon/GainXBaseWeapon.h"

UGainXAnimInstance::UGainXAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    UpdateGroundDistance();
}

void UGainXAnimInstance::UpdateGroundDistance() 
{
    const auto Character = Cast<AGainXBaseCharacter>(GetOwningActor());
    if (!Character) return;

    const auto CharMoveComp = Cast<UGainXCharacterMovementComponent>(Character->GetMovementComponent());
    if (!CharMoveComp) return;

    GroundDistance = CharMoveComp->GetGroundDistance();
}
