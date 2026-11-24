// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InteractionInterface.h"
#include "NpcCharacter.generated.h"

class UInputAction;
class UDialogueDataAsset;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ANpcCharacter : public ABaseCharacter, public IInteractionInterface
{
	GENERATED_BODY()
	
public:

	ANpcCharacter();

	/* Functions */

	/* Interaction Interface */
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual void Interact_Implementation(AActor* InteractorActor) override;
	virtual FInteractionData GetInteractionData_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<const UInputAction> InteractionAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<const UDialogueDataAsset> DialogueData = nullptr;

};
