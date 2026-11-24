// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerHUDWidget.h"
#include "InputMappingContext.h"

#include "MyPlayerController.generated.h"


class UPlayerHUDWidget;
class UUserWidget;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	UPlayerHUDWidget* HUDWidget;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* DialogueWidget;

protected:

	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleDialogueStarted(AActor* Npc);

	UFUNCTION()
	void HandleDialogueEnded();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> DialogueWidgetClass;
	
private:
	
	/* Input Mapping Context */
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TArray<UInputMappingContext*> DefaultMappingContexts;
};
