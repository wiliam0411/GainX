// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/SCircumferenceMarkerWidget.h"
#include "CircumferenceMarkerWidget.generated.h"

/**
 *
 */
UCLASS()
class GAINX_API UCircumferenceMarkerWidget : public UWidget
{
    GENERATED_BODY()

public:
    UCircumferenceMarkerWidget(const FObjectInitializer& ObjectInitializer);

    //~UWidget interface
    virtual void SynchronizeProperties() override;
    virtual TSharedRef<SWidget> RebuildWidget() override;
    //~End of UWidget interface

    //~UVisual interface
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    //~End of UVisual interface

    /* Sets the radius of the circle */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetRadius(float InRadius);

public:
    /* The list of positions/orientations to draw the markers at */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
    TArray<FCircumferenceMarkerEntry> MarkerList;

    /* The radius of the circle */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (ClampMin = 0.0))
    float Radius = 48.0f;

    /* The marker image to place around the circle */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
    FSlateBrush MarkerImage;

    /* Whether reticle corner images are placed outside the spread radius */
    //@TODO: Make this a 0-1 float alignment instead (e.g., inside/on/outside the radius)?
    UPROPERTY(EditAnywhere, Category = Corner)
    uint8 bReticleCornerOutsideSpreadRadius : 1;

private:
    /** Internal slate widget representing the actual marker visuals */
    TSharedPtr<SCircumferenceMarkerWidget> MyMarkerWidget;
};