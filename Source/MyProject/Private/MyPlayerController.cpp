// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "MyProject.h"
#include "PlayerCharacter.h"
#include "DialogueSubsystem.h"


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
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("EnhancedInputLocalPlayerSubsystem is not valid!"));

		}

		// Bind Dialogue Subsystem
		if (UDialogueSubsystem* DialogueSubsystem = ULocalPlayer::GetSubsystem<UDialogueSubsystem>(GetLocalPlayer()))
		{
			DialogueSubsystem->OnDialogueStarted.AddDynamic(this, &AMyPlayerController::HandleDialogueStarted);
			DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &AMyPlayerController::HandleDialogueEnded);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DialogueSubsystem is not valid!"));
		}
	}
}

/* When 100% possessed: */

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsLocalController()) return;

	// Create HUD
	if (HUDWidgetClass && !HUDWidget)
	{
		HUDWidget = CreateWidget<UPlayerHUDWidget>(this, HUDWidgetClass);

		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			HUDWidget->SetCrosshairVisibility(false);
		}
	}

	// Create Dialogue Widget
	if (DialogueWidgetClass && !DialogueWidget)
	{
		DialogueWidget = CreateWidget<UUserWidget>(this, DialogueWidgetClass);

		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport(10);
		}
	}
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UDialogueSubsystem* Dialogue = ULocalPlayer::GetSubsystem<UDialogueSubsystem>(GetLocalPlayer()))
	{
		Dialogue->OnDialogueStarted.RemoveDynamic(this, &AMyPlayerController::HandleDialogueStarted);
		Dialogue->OnDialogueEnded.RemoveDynamic(this, &AMyPlayerController::HandleDialogueEnded);
	}
	Super::EndPlay(EndPlayReason);
}

void AMyPlayerController::HandleDialogueStarted(AActor* Npc)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->SetGameplaySuspended(true);
		PlayerCharacter->DisableInput(this);
	}

	// Cursor
	SetShowMouseCursor(true);

	// Input Mode (Focus on widget)
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	if (DialogueWidget)
	{
		InputMode.SetWidgetToFocus(DialogueWidget->TakeWidget());
	}
	SetInputMode(InputMode);

}

void AMyPlayerController::HandleDialogueEnded()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->EnableInput(this);
		PlayerCharacter->SetGameplaySuspended(false);
	}

	// Cursor
	SetShowMouseCursor(false);

	// Input Mode
	SetInputMode(FInputModeGameOnly());
	FlushPressedKeys();   // a key released while input was disabled must not stay "held"
}
