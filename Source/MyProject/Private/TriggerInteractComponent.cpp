// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerInteractComponent.h"

#include "GameFramework/Actor.h"
#include "Lock.h"
#include "MoverAComponent.h"

// Sets default values for this component's properties
UTriggerInteractComponent::UTriggerInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTriggerInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	Lock = Cast<ALock>(GetOwner());

	if (Lock)
	{
		MoverComp = Lock->FindComponentByClass<UMoverAComponent>();

		if (!MoverComp)
		{
			UE_LOG(LogTemp, Display, TEXT("Mover is missing."));
		}
	}
	
}


// Called every frame
void UTriggerInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UTriggerInteractComponent::CheckTriggerCondition(AActor* Interactor)
{
	if (!Lock) return false;

	if (Lock->GetIsRequiresKeyInventory())
	{
		//Requires checking from inventory (e.g. One-time-use dungeon key)
		if (!bIsTriggered)
		{
			if (Lock->ProcessLockCheckInventory(Interactor)) return true;
		}
	}
	else
	{
		if (!bIsTriggered)
		{
			return true;
		}
	}

	return false;
}

void UTriggerInteractComponent::Trigger(bool bNewTriggerValue, AActor* Interactor)
{
	if (CheckTriggerCondition(Interactor))
	{
		bIsTriggered = bNewTriggerValue;

		if (MoverComp)
		{
			MoverComp->SetShouldMove(bNewTriggerValue);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("%s doesn't have a mover to trigger!"), *GetOwner()->GetActorNameOrLabel());
		}
	}

	
}

