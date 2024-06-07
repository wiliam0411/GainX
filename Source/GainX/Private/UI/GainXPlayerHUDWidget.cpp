// GainX, All Rights Reserved

#include "UI/GainXPlayerHUDWidget.h"
#include "Components/GainXHealthComponent.h"
#include "Components/GainXWeaponComponent.h"
#include "GainXUtils.h"
#include "Components/ProgressBar.h"
#include "Player/GainXPlayerState.h"
#include "GameplayEffect.h"

void UGainXPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UGainXPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UGainXPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = GainXUtils::GetGainXPlayerComponent<UGainXHealthComponent>(NewPawn);
    UpdateHealthBar();
}

void UGainXPlayerHUDWidget::OnHealthChanged(float NewValue, float Delta)
{
    if (Delta < 0.0f)
    {
        OnTakeDamage();

        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}

float UGainXPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = UGainXHealthComponent::FindHealthComponent(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthNormalized();
}

bool UGainXPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = UGainXHealthComponent::FindHealthComponent(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool UGainXPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void UGainXPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}

int32 UGainXPlayerHUDWidget::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer();
    if (!Controller) return 0;

    const auto PlayerState = Cast<AGainXPlayerState>(Controller->PlayerState);
    if (!PlayerState) return 0;

    return PlayerState->GetKillsNum();
}

FString UGainXPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    auto BulletStr = FString::FromInt(BulletsNum);
    const auto SymbolsNumToAdd = MaxLen - BulletStr.Len();

    if (SymbolsNumToAdd > 0)
    {
        BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
    }

    return BulletStr;
}
