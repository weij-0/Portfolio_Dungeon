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
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealthBar(float Percent, float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateManaBar(float Percent, float CurrentMana, float MaxMana);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCrosshairVisibility(bool bIsTargetingPickup);

protected:

	// Called when the widget is constructed (similar to BeginPlay)
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TextBlock_Crosshair;
};
