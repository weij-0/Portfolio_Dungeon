// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ItemBase.h"

FText UInteractionStatics::GetInputKey(const UInputMappingContext* TargetContext, const UInputAction* TargetAction)
{
	if (!TargetContext || !TargetAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetContext or TargetAction is null"));
		return FText::GetEmpty();
	}

	for (const FEnhancedActionKeyMapping& Mapping : TargetContext->GetMappings())
	{
		if (Mapping.Action == TargetAction)
		{
			return Mapping.Key.GetDisplayName();
		}
	}

	return FText::GetEmpty();
}

FText UInteractionStatics::GetInteractionItem(const UItemBase* ItemInstance)
{
	if (ItemInstance && !ItemInstance->TextData.InteractionText.IsEmpty())
	{
		return ItemInstance->TextData.InteractionText;
	}

	return FText::GetEmpty();
}