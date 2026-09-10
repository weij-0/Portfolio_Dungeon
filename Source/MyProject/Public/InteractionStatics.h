// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputAction.h"
#include "InteractionStatics.generated.h"

class UInputMappingContext;
class UItemBase;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "Interaction|UI")
	static FText GetInputKey(const UInputMappingContext* TargetContext, const UInputAction* TargetAction);

	UFUNCTION(BlueprintPure, Category = "Interaction|UI")
	static FText GetInteractionItem(const UItemBase* ItemInstance);
};
