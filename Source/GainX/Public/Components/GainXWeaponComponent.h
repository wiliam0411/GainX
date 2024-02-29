// GainX, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GainXCoreTypes.h"
#include "GainXWeaponComponent.generated.h"

class AGainXBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAINX_API UGainXWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGainXWeaponComponent();

    /* Check if character can fire and call StartFire function of current weapon */
    virtual void StartFire();

    void StopFire();

    /* Get next weapon array index and call equip function */
    virtual void NextWeapon();

    /* Call ChangeClip function */
    void Reload();

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<AGainXBaseWeapon> WeaponType, int32 ClipsAmount);
    bool NeedAmmo(TSubclassOf<AGainXBaseWeapon> WeaponType);

    void Zoom(bool Enabled);

    AGainXBaseWeapon* GetCurrentWeapon() { return CurrentWeapon; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> EquipAnimMontage;

    UPROPERTY()
    AGainXBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<AGainXBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    void EquipWeapon(int32 WeaponIndex);

private:
    UPROPERTY()
    TObjectPtr<UAnimMontage> CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    /* Bind equip and reload anim notify delegates */
    void InitAnimations();

    /* Spawn and bind OnClimpEmpty delegate */
    void SpawnWeapons();

    /* Create attachment rules and attach weapon to character */
    void AttachWeaponToSocket(AGainXBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    /* Call PlayAnimMontage function of ACharacter */
    void PlayAnimMontage(UAnimMontage* Animation);

    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    void OnEmptyClip(AGainXBaseWeapon* AmmoEmptyWeapon);

    void ChangeClip();
};