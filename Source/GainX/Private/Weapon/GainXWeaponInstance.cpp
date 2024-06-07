// GainX, All Rights Reserved

#include "Weapon/GainXWeaponInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UGainXWeaponInstance::UGainXWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGainXWeaponInstance::UpdateWeaponInstance(float DeltaTime)
{
    APawn* Pawn = GetPawn();
    check(Pawn != nullptr);

    const bool bMinSpread = UpdateSpread(DeltaTime);
    const bool bMinMultipliers = UpdateMultipliers(DeltaTime);

    bHasFirstShotAccuracy = bAllowFirstShotAccuracy && bMinMultipliers && bMinSpread;
}

void UGainXWeaponInstance::OnEquipped()
{
    Super::OnEquipped();

    // Start heat in the middle
    float MinHeatRange;
    float MaxHeatRange;
    ComputeHeatRange(/*out*/ MinHeatRange, /*out*/ MaxHeatRange);
    CurrentHeat = (MinHeatRange + MaxHeatRange) * 0.5f;

    // Derive spread
    CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);

    // Default the multipliers to 1x
    CurrentSpreadAngleMultiplier = 1.0f;
    StandingStillMultiplier = 1.0f;
    JumpFallMultiplier = 1.0f;
    CrouchingMultiplier = 1.0f;
}

void UGainXWeaponInstance::OnUnequipped()
{
    Super::OnUnequipped();
}

void UGainXWeaponInstance::AddSpread()
{
    // Sample the heat up curve
    const float HeatPerShot = HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
    CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

    // Map the heat to the spread angle
    CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

void UGainXWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
    HeatToSpreadCurve.GetRichCurveConst()->GetValueRange(/*out*/ MinSpread, /*out*/ MaxSpread);
}

void UGainXWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
    float Min1;
    float Max1;
    HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min1, /*out*/ Max1);

    float Min2;
    float Max2;
    HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min2, /*out*/ Max2);

    float Min3;
    float Max3;
    HeatToSpreadCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min3, /*out*/ Max3);

    MinHeat = FMath::Min(FMath::Min(Min1, Min2), Min3);
    MaxHeat = FMath::Max(FMath::Max(Max1, Max2), Max3);
}

float UGainXWeaponInstance::ClampHeat(float NewHeat)
{
    float MinHeat;
    float MaxHeat;
    ComputeHeatRange(/*out*/ MinHeat, /*out*/ MaxHeat);

    return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
}

bool UGainXWeaponInstance::UpdateSpread(float DeltaSeconds)
{
    const float TimeSinceFired = GetWorld()->TimeSince(LastFireTime);

    if (TimeSinceFired > SpreadRecoveryCooldownDelay)
    {
        const float CooldownRate = HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
        CurrentHeat = ClampHeat(CurrentHeat - (CooldownRate * DeltaSeconds));
        CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
    }

    float MinSpread;
    float MaxSpread;
    ComputeSpreadRange(/*out*/ MinSpread, /*out*/ MaxSpread);

    return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread, KINDA_SMALL_NUMBER);
}

bool UGainXWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
    const float MultiplierNearlyEqualThreshold = 0.05f;

    APawn* Pawn = GetPawn();
    check(Pawn != nullptr);
    UCharacterMovementComponent* CharMovementComp = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());

    // See if we are standing still, and if so, smoothly apply the bonus
    const float PawnSpeed = Pawn->GetVelocity().Size();
    const float MovementTargetValue = FMath::GetMappedRangeValueClamped(
        /*InputRange=*/FVector2D(StandingStillSpeedThreshold, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange),
        /*OutputRange=*/FVector2D(SpreadAngleMultiplier_StandingStill, 1.0f),
        /*Alpha=*/PawnSpeed);
    StandingStillMultiplier = FMath::FInterpTo(StandingStillMultiplier, MovementTargetValue, DeltaSeconds, TransitionRate_StandingStill);
    const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(StandingStillMultiplier, SpreadAngleMultiplier_StandingStill, SpreadAngleMultiplier_StandingStill * 0.1f);

    // See if we are crouching, and if so, smoothly apply the bonus
    const bool bIsCrouching = (CharMovementComp != nullptr) && CharMovementComp->IsCrouching();
    const float CrouchingTargetValue = bIsCrouching ? SpreadAngleMultiplier_Crouching : 1.0f;
    CrouchingMultiplier = FMath::FInterpTo(CrouchingMultiplier, CrouchingTargetValue, DeltaSeconds, TransitionRate_Crouching);
    const bool bCrouchingMultiplierAtTarget = FMath::IsNearlyEqual(CrouchingMultiplier, CrouchingTargetValue, MultiplierNearlyEqualThreshold);

    // See if we are in the air (jumping/falling), and if so, smoothly apply the penalty
    const bool bIsJumpingOrFalling = (CharMovementComp != nullptr) && CharMovementComp->IsFalling();
    const float JumpFallTargetValue = bIsJumpingOrFalling ? SpreadAngleMultiplier_JumpingOrFalling : 1.0f;
    JumpFallMultiplier = FMath::FInterpTo(JumpFallMultiplier, JumpFallTargetValue, DeltaSeconds, TransitionRate_JumpingOrFalling);
    const bool bJumpFallMultiplerIs1 = FMath::IsNearlyEqual(JumpFallMultiplier, 1.0f, MultiplierNearlyEqualThreshold);

    // Determine if we are aiming down sights, and apply the bonus based on how far into the camera transition we are
    float AimingAlpha = 1.0f;

    const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
        /*InputRange=*/FVector2D(0.0f, 1.0f),
        /*OutputRange=*/FVector2D(1.0f, SpreadAngleMultiplier_Aiming),
        /*Alpha=*/AimingAlpha);
    const bool bAimingMultiplierAtTarget = FMath::IsNearlyEqual(AimingMultiplier, SpreadAngleMultiplier_Aiming, KINDA_SMALL_NUMBER);

    // Combine all the multipliers
    const float CombinedMultiplier = AimingMultiplier * StandingStillMultiplier * CrouchingMultiplier * JumpFallMultiplier;
    CurrentSpreadAngleMultiplier = CombinedMultiplier;

    // need to handle these spread multipliers indicating we are not at min spread
    return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtTarget && bJumpFallMultiplerIs1 && bAimingMultiplierAtTarget;
}
