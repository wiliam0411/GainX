// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbility_RangedWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Equipment/GainXWeaponActor.h"

UGainXAbility_RangedWeapon::UGainXAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

AGainXWeaponActor* UGainXAbility_RangedWeapon::GetWeaponActor() const
{
    return Cast<AGainXWeaponActor>(GetAssociatedEquipmentActor(AGainXEquipmentActor::StaticClass()));
}

FHitResult UGainXAbility_RangedWeapon::TraceSingleBullet(const FVector& StartTrace, const FVector& EndTrace) const
{
    TArray<FHitResult> HitResults;
    const ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel2;
    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(TraceSingleBullet), true, GetAvatarActorFromActorInfo());

    GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);

    FHitResult Hit(ForceInit);

    if (HitResults.Num() > 0)
    {
        Hit = HitResults.Last();
    }

    else
    {
        Hit.TraceStart = StartTrace;
        Hit.TraceEnd = EndTrace;
        Hit.Location = EndTrace;
        Hit.ImpactPoint = EndTrace;
    }

    return Hit;
}

void UGainXAbility_RangedWeapon::TraceCartridge(FVector StartTrace, FVector AimDirection, TArray<FHitResult>& OutHits)
{
    const AGainXWeaponActor* WeaponInstance = GetWeaponActor();
    check(WeaponInstance);

    for (int32 BulletIndex = 0; BulletIndex < WeaponInstance->GetBulletsPerCartridge(); ++BulletIndex)
    {
        // Here we calculating spread
        const float BaseSpreadAngle = WeaponInstance->GetCalculatedSpreadAngle();
        const float SpreadAngleMultiplier = WeaponInstance->GetCalculatedSpreadAngleMultiplier();
        const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;
        const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);
        const FVector BulletDir = VRandConeNormalDistribution(AimDirection, HalfSpreadAngleInRadians, WeaponInstance->GetSpreadExponent());

        const FVector EndTrace = StartTrace + BulletDir * WeaponInstance->GetMaxDamageRange();

        OutHits.Add(TraceSingleBullet(StartTrace, EndTrace));
    }
}

//@TODO: Should do more complicated logic here when the player is close to a wall, etc...
void UGainXAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
    if (const APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo()))
    {
        FVector CameraLocation = AvatarPawn->GetActorLocation();
        FRotator CameraRotation = AvatarPawn->GetControlRotation();

        if (const APlayerController* AvatarPlayerController = Cast<APlayerController>(AvatarPawn->Controller))
        {
            AvatarPlayerController->GetPlayerViewPoint(/*out*/ CameraLocation, /*out*/ CameraRotation);
        }

        const FTransform CameraTransform = FTransform(CameraRotation, CameraLocation);
        FVector StartTrace = CameraTransform.GetTranslation();
        FVector AimDir = CameraTransform.GetUnitAxis(EAxis::X);

        TraceCartridge(StartTrace, AimDir, OutHits);
    }
}

FVector UGainXAbility_RangedWeapon::VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
    if (ConeHalfAngleRad > 0.f)
    {
        const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

        // consider the cone a concatenation of two rotations. one "away" from the center line, and another "around" the circle
        // apply the exponent to the away-from-center rotation. a larger exponent will cluster points more tightly around the center
        const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
        const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
        const float AngleAround = FMath::FRand() * 360.0f;

        FRotator Rot = Dir.Rotation();
        FQuat DirQuat(Rot);
        FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
        FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
        FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
        FinalDirectionQuat.Normalize();

        return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
    }
    else
    {
        return Dir.GetSafeNormal();
    }
}

void UGainXAbility_RangedWeapon::StartRangedWeaponTargeting()
{
    TArray<FHitResult> FoundHits;
    PerformLocalTargeting(/*out*/ FoundHits);

    FGameplayAbilityTargetDataHandle TargetData;
    for (const FHitResult& FoundHit : FoundHits)
    {
        FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
        NewTargetData->HitResult = FoundHit;
        TargetData.Add(NewTargetData);
    }

    check(GetWeaponActor());
    GetWeaponActor()->AddSpread();

    OnRangedWeaponTargetDataReady(TargetData);
}

void UGainXAbility_RangedWeapon::GetHitResultsFromTargetData(const FGameplayAbilityTargetDataHandle& InData, TArray<FHitResult>& HitResults) const
{
    HitResults.Empty();

    for (int32 Index = 0; Index < InData.Num(); ++Index)
    {
        FGameplayAbilityTargetData* CurrentTargetData = InData.Data[Index].Get();
        HitResults.Add(*CurrentTargetData->GetHitResult());
    }
}
