// GainX, All Rights Reserved


#include "Components/GainXCharacterMovementComponent.h"
#include "Player/GainXBaseCharacter.h"

float UGainXCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AGainXBaseCharacter* Player = Cast<AGainXBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunMultiplier : MaxSpeed;
}