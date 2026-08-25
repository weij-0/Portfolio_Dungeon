// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

void UPlayerHUDWidget::UpdateHealthBar(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f , 1.0f));
	}
}

void UPlayerHUDWidget::UpdateStaminaBar(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}

void UPlayerHUDWidget::SetCrosshairVisibility(bool bIsTargetingPickup)
{
	bIsTargetingPickup ? TextBlock_Crosshair->SetVisibility(ESlateVisibility::Visible) : TextBlock_Crosshair->SetVisibility(ESlateVisibility::Collapsed);
}
