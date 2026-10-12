// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaWidget.h"
#include "GameFramework/Character.h"
#include "StaminaComponent.h"

void UStaminaWidget::UpdateStaminaWidget(float Percent, float CurrentStamina, float MaxStamina)
{
	if (CachedMaterialInstanceDynamic)
	{
		CachedMaterialInstanceDynamic->SetScalarParameterValue("Percent", FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StaminaRadialImage)
	{
		CachedMaterialInstanceDynamic = StaminaRadialImage->GetDynamicMaterial();
	}

	if (ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn()))
	{
		/* Stamina Component */
		if (UStaminaComponent* StaminaComp = Character->FindComponentByClass<UStaminaComponent>())
		{
			// Bind the OnStaminaChanged event to the UpdateStaminaWidget function
			StaminaComp->OnStaminaChanged.AddUniqueDynamic(this, &UStaminaWidget::UpdateStaminaWidget);
			
			/* Initialization */
			UpdateStaminaWidget(StaminaComp->GetCurrentStaminaPercent(), StaminaComp->GetCurrentStamina(), StaminaComp->GetMaxStamina());
		}
	}
}

void UStaminaWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// Unbind delegate when widget is destroyed or recreated to prevent dangling references
	if (ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn()))
	{
		if (UStaminaComponent* StaminaComp = Character->FindComponentByClass<UStaminaComponent>())
		{
			StaminaComp->OnStaminaChanged.RemoveDynamic(this, &UStaminaWidget::UpdateStaminaWidget);
		}
	}

	
}
