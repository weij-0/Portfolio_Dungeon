// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "InteractableComponent.generated.h"

class UInteractionWidget;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableInteracted, AActor*, Interactor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteractableComponent : public USceneComponent, public IInteractionInterface, public IInteractionFocusListener
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	/* IInteractionInterface */
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FInteractionData GetInteractionData_Implementation() const override;

	/* IInteractionFocusListener */
	virtual void OnFocusGained_Implementation(AActor* Interactor) override;
	virtual void OnFocusLost_Implementation(AActor* Interactor) override;

	// Refreshes the interaction prompt widget with the latest data
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RefreshPrompt();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionEnabled(bool bNewEnabled);

	// Fired on interaction
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractableInteracted OnInteract;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Properties */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<UInteractionWidget> PromptWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bInteractionEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FInteractionData Data;

private:	
	
	void SetPromptVisible(bool bShowPrompt);

	void InitializePromptWidget();

	TObjectPtr<UWidgetComponent> PromptWidgetComponent;

	TWeakObjectPtr<AActor> CurrentInteractor;

	bool bIsFocused = false;

};
