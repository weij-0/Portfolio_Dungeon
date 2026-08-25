// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaWidget.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StaminaRadialImage)
	{
		CachedMaterialInstanceDynamic = StaminaRadialImage->GetDynamicMaterial();
	}
}

void UStaminaWidget::UpdateStaminaWidget(float Percent)
{
	if (CachedMaterialInstanceDynamic)
	{
		CachedMaterialInstanceDynamic->SetScalarParameterValue("Percent", Percent);
	}
}
