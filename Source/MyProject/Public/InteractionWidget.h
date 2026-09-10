// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionData.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionData(const FInteractionData& NewInteractionData);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	FInteractionData InteractionData;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void RefreshInteractionDisplay();
};
