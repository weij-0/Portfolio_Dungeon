// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcCharacter.h"
#include "DialogueSubsystem.h"
#include "InteractTraceComponent.h"

ANpcCharacter::ANpcCharacter()
{
}

bool ANpcCharacter::CanInteract_Implementation(AActor* Interactor) const
{
	if (!DialogueData) return false;

	UDialogueSubsystem* DialogueSubsystem = UDialogueSubsystem::Get(Interactor);
	
	if (!DialogueSubsystem) return false;

	return DialogueSubsystem && !DialogueSubsystem->GetIsDialogueActive();
}

void ANpcCharacter::Interact_Implementation(AActor* InteractorActor)
{
	UDialogueSubsystem* DialogueSubsystem = UDialogueSubsystem::Get(InteractorActor);
	
	if (DialogueSubsystem) 
	{
		if (!DialogueSubsystem->StartDialogue(this, DialogueData))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to start dialogue with NPC: %s"), *GetName());
		}
	}
}

FInteractionData ANpcCharacter::GetInteractionData_Implementation() const
{
	FInteractionData Data;

	Data.InteractionText = FText::FromString("Talk to NPC");
	//Data.DisplayName = FText::FromString("");
	Data.InputAction = InteractionAction;

	return Data;
}
