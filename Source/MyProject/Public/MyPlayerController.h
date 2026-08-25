// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerHUDWidget.h"
#include "InputMappingContext.h"

#include "MyPlayerController.generated.h"


class UPlayerHUDWidget;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> HUDWidgetClass;
	
private:
	
	/* Input Mapping Context */
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TArray<UInputMappingContext*> DefaultMappingContexts;

public:

	UPROPERTY(VisibleAnywhere)
	UPlayerHUDWidget* HUDWidget;
};
