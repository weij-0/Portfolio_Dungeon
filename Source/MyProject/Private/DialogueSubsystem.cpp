	// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSubsystem.h"
#include "DialogueDataAsset.h"

bool UDialogueSubsystem::StartDialogue(AActor* Npc, const UDialogueDataAsset* DialogueData)
{
	if (bDialogueActive || !Npc || !DialogueData) return false;

	if (!DialogueData->GetDialogueNodeById(DialogueData->StartNodeId))
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogueData %s does not have a valid start node."), *DialogueData->GetName());
		return false;
	}

	// Set the current dialogue state
	bDialogueActive = true;
	CurrentDialogueData = DialogueData;
	CurrentNpc = Npc;
	CurrentNodeId = DialogueData->StartNodeId;
			
	// Start the dialogue with the provided NPC and dialogue data
	OnDialogueStarted.Broadcast(Npc);
	GoToNode(DialogueData->StartNodeId);

	return true;
	
}

void UDialogueSubsystem::AdvanceDialogue()
{
	if (!bDialogueActive || !CurrentDialogueData) return;

	const FDialogueNode* CurrentNode = CurrentDialogueData->GetDialogueNodeById(CurrentNodeId);

	// If node with choices, waits for MakeChoice
	if (!CurrentNode->Choices.IsEmpty()) return;

	// If current node is null or no NEXT node
	if (!CurrentNode || CurrentNode->NextNodeId == NAME_None)
	{
		EndDialogue();
		return;
	}

	// Go to NEXT node
	GoToNode(CurrentNode->NextNodeId);
}

void UDialogueSubsystem::GoToNode(FName NewNodeId)
{
	if (!bDialogueActive || !CurrentDialogueData) return;

	const FDialogueNode* NewNode = CurrentDialogueData->GetDialogueNodeById(NewNodeId);

	if (NewNode)
	{
		// Set the current dialogue state
		CurrentNodeId = NewNodeId;

		OnDialogueNodeChanged.Broadcast(*NewNode);
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueSubsystem::MakeChoice(int32 ChoiceIndex)
{
	if (!bDialogueActive || !CurrentDialogueData) return;

	const FDialogueNode* CurrentNode = CurrentDialogueData->GetDialogueNodeById(CurrentNodeId);

	if (!CurrentNode || !CurrentNode->Choices.IsValidIndex(ChoiceIndex)) return;
	
	GoToNode(CurrentNode->Choices[ChoiceIndex].NextNodeId);
}

void UDialogueSubsystem::EndDialogue()
{
	// Reset the dialogue state
	bDialogueActive = false;
	CurrentNpc = nullptr;
	CurrentDialogueData = nullptr;
	CurrentNodeId = NAME_None;

	OnDialogueEnded.Broadcast();
}

UDialogueSubsystem* UDialogueSubsystem::Get(const AActor* PlayerActor)
{
	const APlayerController* PlayerController = PlayerActor ? Cast<APlayerController>(PlayerActor->GetInstigatorController()) : nullptr;

	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			return LocalPlayer->GetSubsystem<UDialogueSubsystem>();
		}
	}

	return nullptr;
}

