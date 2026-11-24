// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "DialogueSubsystem.generated.h"

class UDialogueDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, AActor*, Npc);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNodeChanged, const FDialogueNode&, Node);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDialogueSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool StartDialogue(AActor* Npc, const UDialogueDataAsset* DialogueData);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void AdvanceDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void GoToNode(FName NewNodeId);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void MakeChoice(int32 ChoiceIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

	/* Setter & Getter */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	static UDialogueSubsystem* Get(const AActor* PlayerActor);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool GetIsDialogueActive() { return bDialogueActive; }

	/* Multicast Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueStarted OnDialogueStarted;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueNodeChanged OnDialogueNodeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

protected:

private: 

	UPROPERTY()
	const UDialogueDataAsset* CurrentDialogueData;

	UPROPERTY()
	FName CurrentNodeId;

	UPROPERTY()
	AActor* CurrentNpc;

	UPROPERTY()
	bool bDialogueActive = false;
};
