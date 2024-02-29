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

    const auto Character = Cast<AGainXBaseCharacter>(GetOwningActor());
    if (!Character) return;

    const auto CharMoveComp = Cast<UGainXCharacterMovementComponent>(Character->GetMovementComponent());
    if (!CharMoveComp) return;

    GroundDistance = CharMoveComp->GetGroundDistance();

    const auto WeaponComponent = Cast<UGainXWeaponComponent>(Character->GetWeaponComponent());
    if (!WeaponComponent) return;

    const auto CurrentWeapon = Cast<AGainXBaseWeapon>(WeaponComponent->GetCurrentWeapon());
    if (!CurrentWeapon) return;

    FTransform SocketWorldTransform = CurrentWeapon->GetWeaponMesh()->GetSocketTransform(LeftHandSocketName);

    FVector OutPosition;
    FRotator OutRotation;
    Character->GetMesh()->TransformToBoneSpace(  //
        RightHandBoneName,                       //
        SocketWorldTransform.GetLocation(),      //
        SocketWorldTransform.Rotator(),          //
        OutPosition,                             //
        OutRotation);                            //

    LeftHandSocketTransform = FTransform(OutRotation, OutPosition, FVector::OneVector);
}