// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "StaminaWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Stamina Widget")
	void UpdateStaminaWidget(float Percent, float CurrentStamina, float MaxStamina);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* StaminaRadialImage;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* CachedMaterialInstanceDynamic;

	
};
