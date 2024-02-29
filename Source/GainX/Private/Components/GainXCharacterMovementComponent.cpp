// GainX, All Rights Reserved

#include "Components/GainXCharacterMovementComponent.h"
#include "Player/GainXBaseCharacter.h"
#include "Components/CapsuleComponent.h"

float UGainXCharacterMovementComponent::GetGroundDistance()
{
    if (!CharacterOwner || MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking) return 0.0f;

    const auto CapsuleComp = CharacterOwner->GetCapsuleComponent();
    if (!CapsuleComp) return 0.0f;

    const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
    const ECollisionChannel CollisionChannel = UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn;

    const FVector TraceStart(GetActorLocation());
    const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - GroundTraceDistance - CapsuleHalfHeight));

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(GainXCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
    FCollisionResponseParams ResponseParam;
    InitCollisionParams(QueryParams, ResponseParam);

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

    return FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
}