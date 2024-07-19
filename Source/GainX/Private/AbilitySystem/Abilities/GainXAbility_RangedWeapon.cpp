// GainX, All Rights Reserved

#include "AbilitySystem/Abilities/GainXAbility_RangedWeapon.h"
#include "Weapon/GainXWeaponInstance.h"

UGainXAbility_RangedWeapon::UGainXAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UGainXWeaponInstance* UGainXAbility_RangedWeapon::GetWeaponInstance() const
{
    return Cast<UGainXWeaponInstance>(GetAssociatedEquipment());
}

FHitResult UGainXAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, TArray<FHitResult>& OutHitResults) const
{
    TArray<FHitResult> HitResults;

    const ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel2;

    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());

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

void UGainXAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits)
{
    UGainXWeaponInstance* WeaponData = InputData.EquipmentWeapon;
    check(WeaponData);

    const int32 BulletsPerCartridge = WeaponData->GetBulletsPerCartridge();

    for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
    {
        const float BaseSpreadAngle = WeaponData->GetCalculatedSpreadAngle();

        const float SpreadAngleMultiplier = WeaponData->GetCalculatedSpreadAngleMultiplier();

        const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;

        const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);

        const FVector BulletDir = VRandConeNormalDistribution(InputData.AimDir, HalfSpreadAngleInRadians, WeaponData->GetSpreadExponent());

        const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->GetMaxDamageRange());

        TArray<FHitResult> AllImpacts;

        FHitResult Impact = WeaponTrace(InputData.StartTrace, EndTrace, AllImpacts);

        OutHits.Add(Impact);
    }
}

//@TODO: Should do more complicated logic here when the player is close to a wall, etc...
void UGainXAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
    const auto AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
    if (!AvatarPawn || !AvatarPawn->IsLocallyControlled())
    {
        return;
    }

    UGainXWeaponInstance* WeaponData = GetWeaponInstance();
    if (!WeaponData)
    {
        return;
    }

    FVector CamLoc;
    FRotator CamRot;

    const auto AvatarPlayerController = Cast<APlayerController>(AvatarPawn->Controller);
    if (AvatarPlayerController)
    {
        AvatarPlayerController->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);
    }
    else
    {
        CamLoc = AvatarPawn->GetActorLocation();
        CamRot = AvatarPawn->GetControlRotation();
    }

    const auto TargetTransform = FTransform(CamRot, CamLoc);

    FRangedWeaponFiringInput FiringInputData;
    FiringInputData.EquipmentWeapon = WeaponData;
    FiringInputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
    FiringInputData.StartTrace = TargetTransform.GetTranslation();
    FiringInputData.EndAim = FiringInputData.StartTrace + FiringInputData.AimDir * WeaponData->GetMaxDamageRange();

    TraceBulletsInCartridge(FiringInputData, OutHits);
}

void UGainXAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData)
{

    // Take ownership of the target data to make sure no callbacks into game code invalidate it out from under us
    FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

    // See if we still have ammo
    if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
    {
        // We fired the weapon, add spread
        UGainXWeaponInstance* WeaponData = GetWeaponInstance();
        check(WeaponData);
        WeaponData->AddSpread();

        // Blueprint Implementable Event
        OnRangedWeaponTargetDataReady(LocalTargetDataHandle);
    }
    else
    {
        K2_EndAbility();
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

    if (FoundHits.Num() > 0)
    {
        for (const FHitResult& FoundHit : FoundHits)
        {
            FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
            NewTargetData->HitResult = FoundHit;
            TargetData.Add(NewTargetData);
        }
    }

    OnTargetDataReadyCallback(TargetData);
}

FGameplayCueParameters UGainXAbility_RangedWeapon::MakeGameplayCueParametersFromTargetData(const FGameplayAbilityTargetDataHandle& InData)
{
    TArray<FVector> HitPosArray;
    TArray<FVector> HitNorArray;

    for (int32 Index = 0; Index < InData.Num(); ++Index)
    {
        const auto CurrentTargetData = static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(InData.Get(Index));
        const FVector HitPos = CurrentTargetData->GetHitResult()->ImpactPoint;
        const FVector HitNor = CurrentTargetData->GetHitResult()->ImpactNormal;
        HitPosArray.Add(HitPos);
        HitNorArray.Add(HitNor);
    }

    auto TracerData = NewObject<UGainXTracerData>();
    TracerData->SetHitPositions(HitPosArray);
    TracerData->SetHitNormals(HitNorArray);

    FGameplayCueParameters CueParameters;
    CueParameters.SourceObject = TracerData;

    return CueParameters;
}
