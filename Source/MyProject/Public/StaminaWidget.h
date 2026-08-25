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

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* StaminaRadialImage;

private:

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* CachedMaterialInstanceDynamic;

public:

	void UpdateStaminaWidget(float Percent);	
};
