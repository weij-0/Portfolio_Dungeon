// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerInteractComponent.generated.h"

class ALock;
class UMoverAComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UTriggerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerInteractComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Functions */

	// Trigger function
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void Trigger(bool bNewTriggerValue, AActor* Interactor);

	bool GetIsTriggered() const { return bIsTriggered; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/* Functions */

	bool CheckTriggerCondition(AActor* Interactor);

	/* Components */

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ALock> Lock;

	UPROPERTY()
	TObjectPtr<UMoverAComponent> MoverComp;

	/* Properties */

	UPROPERTY(VisibleAnywhere)
	bool bIsTriggered = false;
		
};
