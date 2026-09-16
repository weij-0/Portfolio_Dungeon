// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "PlayerCharacter.h"
#include "HealthComponent.h"

void UPlayerHUDWidget::UpdateHealthBar(float Percent, float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f , 1.0f));
	}
}

void UPlayerHUDWidget::UpdateManaBar(float Percent)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}

void UPlayerHUDWidget::SetCrosshairVisibility(bool bIsTargetingPickup)
{
	bIsTargetingPickup ? TextBlock_Crosshair->SetVisibility(ESlateVisibility::Visible) : TextBlock_Crosshair->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UHealthComponent* HealthComp = PlayerChar->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &UPlayerHUDWidget::UpdateHealthBar);

			/* Initialization */

			UpdateHealthBar(HealthComp->GetCurrentHealthPercent(), HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
		}
	}
}
