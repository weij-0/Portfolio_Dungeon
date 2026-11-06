// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractTraceComponent.generated.h"

class UInteractableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteractTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractTraceComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Functions */

	// Called when the player presses the interact button
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool TryInteract();

	// Clears the current focus
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ClearFocus();

	// Get Focused Interactable Component
	UFUNCTION(BlueprintPure, Category = "Interaction")
	UInteractableComponent* GetFocusedInteractable() const { return FocusedInteractable.Get(); }

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Properties */

	// Max Interaction Distance (from Pawn)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0"))
	float InteractionTraceDistance = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

private:
	
	/* Functions */
	void PerformInteractionTrace();

	float CalculateInteractionScore(const UInteractableComponent* Candidate, const FVector& PawnLoc,
		const FVector& CamLoc, const FVector& CamDir) const;

	void SetFocus(UInteractableComponent* NewFocus);

	/* Properties */

	// The actor the trace hit
	TWeakObjectPtr<UInteractableComponent> FocusedInteractable;

};
