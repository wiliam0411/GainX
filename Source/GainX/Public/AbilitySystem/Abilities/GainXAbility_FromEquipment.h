// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GainXGameplayAbility.h"
#include "GainXAbility_FromEquipment.generated.h"

class UGainXEquipmentObject;
class UGainXInventoryItemDefinition;

UCLASS(Blueprintable, BlueprintType)
class GAINX_API UGainXAbility_FromEquipment : public UGainXGameplayAbility
{
    GENERATED_BODY()

public:
    UGainXAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    UGainXEquipmentObject* GetAssociatedEquipment() const;

    UFUNCTION(BlueprintCallable, Category = "GainX|Ability")
    UGainXInventoryItemDefinition* GetAssociatedItem() const;
};
