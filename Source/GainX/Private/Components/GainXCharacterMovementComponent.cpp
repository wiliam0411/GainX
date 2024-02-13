// GainX, All Rights Reserved

#include "Components/GainXCharacterMovementComponent.h"
#include "Player/GainXBaseCharacter.h"
#include "Components/CapsuleComponent.h"

float UGainXCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AGainXBaseCharacter* Player = Cast<AGainXBaseCharacter>(GetPawnOwner());
    return MaxSpeed;
}

float UGainXCharacterMovementComponent::GetGroundDistance()
{
    if (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking) return 0.0f;

    if (!CharacterOwner) return 0.0f;

    const auto CapsuleComp = CharacterOwner->GetCapsuleComponent();
    if (!CapsuleComp) return 0.0f;

    const auto CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
    const auto CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);

    const FVector TraceStart(GetActorLocation());
    const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - GroundTraceDistance - CapsuleHalfHeight));

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(GainXCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
    FCollisionResponseParams ResponseParam;
    InitCollisionParams(QueryParams, ResponseParam);

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

    if (!HitResult.bBlockingHit) return 0.0f;

    return FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
}