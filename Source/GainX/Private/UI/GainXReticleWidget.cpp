// GainX, All Rights Reserved

#include "UI/GainXReticleWidget.h"
#include "Inventory/GainXInventoryItem.h"
#include "Weapon/GainXWeaponInstance.h"

UGainXReticleWidget::UGainXReticleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

float UGainXReticleWidget::ComputeSpreadAngle() const
{
    if (!WeaponInstance)
    {
        return 0.0f;
    }

    const float BaseSpreadAngle = WeaponInstance->GetCalculatedSpreadAngle();
    const float SpreadAngleMultiplier = WeaponInstance->GetCalculatedSpreadAngleMultiplier();
    const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;

    return ActualSpreadAngle;
}

float UGainXReticleWidget::ComputeMaxScreenspaceSpreadRadius() const
{
    const float LongShotDistance = 10000.f;

    APlayerController* PC = GetOwningPlayer();
    if (PC && PC->PlayerCameraManager)
    {
        // A weapon's spread can be thought of as a cone shape. To find the screenspace spread for reticle visualization,
        // we create a line on the edge of the cone at a long distance. The end of that point is on the edge of the cone's circle.
        // We then project it back onto the screen. Its distance from screen center is the spread radius.

        // This isn't perfect, due to there being some distance between the camera location and the gun muzzle.

        const float SpreadRadiusRads = FMath::DegreesToRadians(ComputeSpreadAngle() * 0.5f);
        const float SpreadRadiusAtDistance = FMath::Tan(SpreadRadiusRads) * LongShotDistance;

        FVector CamPos;
        FRotator CamOrient;
        PC->PlayerCameraManager->GetCameraViewPoint(CamPos, CamOrient);

        FVector CamForwDir = CamOrient.RotateVector(FVector::ForwardVector);
        FVector CamUpDir = CamOrient.RotateVector(FVector::UpVector);

        FVector OffsetTargetAtDistance = CamPos + (CamForwDir * LongShotDistance) + (CamUpDir * SpreadRadiusAtDistance);

        FVector2D OffsetTargetInScreenspace;

        if (PC->ProjectWorldLocationToScreen(OffsetTargetAtDistance, OffsetTargetInScreenspace, true))
        {
            int32 ViewportSizeX(0), ViewportSizeY(0);
            PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

            const FVector2D ScreenSpaceCenter(FVector::FReal(ViewportSizeX) * 0.5f, FVector::FReal(ViewportSizeY) * 0.5f);

            return (OffsetTargetInScreenspace - ScreenSpaceCenter).Length();
        }
    }

    return 0.0f;
}

bool UGainXReticleWidget::HasFirstShotAccuracy() const
{
    if (!WeaponInstance)
    {
        return false;
    }

    return WeaponInstance->HasFirstShotAccuracy();
}

void UGainXReticleWidget::InitializeFromWeapon(UGainXWeaponInstance* InWeapon)
{
    WeaponInstance = InWeapon;
    InventoryItem = nullptr;
    if (WeaponInstance)
    {
        InventoryItem = Cast<UGainXInventoryItem>(WeaponInstance->GetInstigator());
    }
    OnWeaponInitialized();
}