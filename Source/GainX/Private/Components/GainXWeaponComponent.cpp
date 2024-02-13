// GainX, All Rights Reserved

#include "Components/GainXWeaponComponent.h"
#include "Weapon/GainXBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/GainXEquipFinishedAnimNotify.h"
#include "Animations/GainXReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UGainXWeaponComponent::UGainXWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGainXWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UGainXWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    Super::EndPlay(EndPlayReason);
}

void UGainXWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<AGainXBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &UGainXWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UGainXWeaponComponent::AttachWeaponToSocket(AGainXBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UGainXWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        CurrentWeapon->Zoom(false);
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                 //
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void UGainXWeaponComponent::StartFire()
{
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void UGainXWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void UGainXWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UGainXWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void UGainXWeaponComponent::InitAnimations()
{
    if (WeaponData.Num() == 0) return;
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UGainXEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UGainXWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UGainXReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &UGainXWeaponComponent::OnReloadFinished);
    }
}

void UGainXWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;
    EquipAnimInProgress = false;
}

void UGainXWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;
    ReloadAnimInProgress = false;
}

bool UGainXWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UGainXWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UGainXWeaponComponent::CanReload() const
{
    return CurrentWeapon             //
           && !EquipAnimInProgress   //
           && !ReloadAnimInProgress  //
           && CurrentWeapon->CanReload();
}

void UGainXWeaponComponent::Reload()
{
    ChangeClip();
}

bool UGainXWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool UGainXWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool UGainXWeaponComponent::TryToAddAmmo(TSubclassOf<AGainXBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

bool UGainXWeaponComponent::NeedAmmo(TSubclassOf<AGainXBaseWeapon> WeaponType)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}

void UGainXWeaponComponent::Zoom(bool Enabled)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom(Enabled);
    }
}

void UGainXWeaponComponent::OnEmptyClip(AGainXBaseWeapon* AmmoEmptyWeapon)
{
    if (!AmmoEmptyWeapon) return;

    if (CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip();
    }
    else
    {
        for (const auto Weapon : Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}

void UGainXWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}