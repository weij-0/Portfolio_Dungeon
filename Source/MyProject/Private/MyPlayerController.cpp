// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "MyProject.h"
#include "PlayerCharacter.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Verify controller is from local player 
	if (IsLocalController())
	{

		// Add Input Mapping Context (IMC)
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				if (CurrentContext)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
				
			}
		}
	}
}

/* When 100% possessed: */

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Create HUD
	if (IsLocalController() && HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UPlayerHUDWidget>(this, HUDWidgetClass);

		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			HUDWidget->SetCrosshairVisibility(false);
		}
	}
}
