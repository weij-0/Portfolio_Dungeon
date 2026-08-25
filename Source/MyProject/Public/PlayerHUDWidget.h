// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void UpdateHealthBar(float Percent);

	void UpdateStaminaBar(float Percent);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCrosshairVisibility(bool bIsTargetingPickup);

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Crosshair;
};
