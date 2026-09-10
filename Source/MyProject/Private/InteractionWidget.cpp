// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

void UInteractionWidget::SetInteractionData(const FInteractionData& NewInteractionData)
{
	InteractionData = NewInteractionData;

	RefreshInteractionDisplay();
}
