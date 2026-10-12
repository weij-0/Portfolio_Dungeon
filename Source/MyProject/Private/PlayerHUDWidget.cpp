// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "ManaComponent.h"

void UPlayerHUDWidget::UpdateHealthBar(float Percent, float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f , 1.0f));
	}
}

void UPlayerHUDWidget::UpdateManaBar(float Percent, float CurrentMana, float MaxMana)
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

	if (ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn()))
	{
		/* Health Component */
		if (UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->OnHealthChanged.AddUniqueDynamic(this, &UPlayerHUDWidget::UpdateHealthBar);

			/* Initialization */

			UpdateHealthBar(HealthComp->GetCurrentHealthPercent(), HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
		}

		/* Mana Component */
		if (UManaComponent* ManaComp = Character->FindComponentByClass<UManaComponent>())
		{
			ManaComp->OnManaChanged.AddUniqueDynamic(this, &UPlayerHUDWidget::UpdateManaBar);

			/* Initialization */
			UpdateManaBar(ManaComp->GetCurrentManaPercent(), ManaComp->GetCurrentMana(), ManaComp->GetMaxMana());
		}
	}
}

void UPlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn()))
	{
		/* Health Component */
		if (UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->OnHealthChanged.RemoveDynamic(this, &UPlayerHUDWidget::UpdateHealthBar);
		}
		/* Mana Component */
		if (UManaComponent* ManaComp = Character->FindComponentByClass<UManaComponent>())
		{
			ManaComp->OnManaChanged.RemoveDynamic(this, &UPlayerHUDWidget::UpdateManaBar);
		}
	}
}
