// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxComponent.h"

#include "GameFramework/Actor.h"
#include "Lock.h"
#include "MoverAComponent.h"

UTriggerBoxComponent::UTriggerBoxComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

	AcceptableActorTags.Add(FName("Player"));
}

void UTriggerBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsTriggered = false;

	Lock = Cast<ALock>(GetOwner());

	/* Lock & Mover */
	if (Lock)
	{
		Mover = Lock->FindComponentByClass<UMoverAComponent>();

		if (!Mover)
		{
			UE_LOG(LogTemp, Display, TEXT("Mover is missing."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Lock is missing."));
	}

	if (Lock && Mover)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerBoxComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerBoxComponent::OnOverlapEnd);
	}

}

void UTriggerBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UTriggerBoxComponent::Trigger(bool NewTriggerValue)
{
	bIsTriggered = NewTriggerValue;

	if (Mover)
	{
		Mover->SetShouldMove(NewTriggerValue);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s doesn't have a mover to trigger!"), *GetOwner()->GetActorNameOrLabel());
	}
}

/* Event-Driven Primitive Delegates for Overlap Events */

void UTriggerBoxComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!Lock || !OtherActor || OtherActor == GetOwner()) return;

	if (Lock->GetIsRequiresKeyInventory())
	{
		if (HasAcceptableTag(OtherActor))
		{
			//Requires checking from inventory (e.g. One-time-use dungeon key)
			if (!bIsTriggered)
			{
				if (Lock->ProcessLockCheckInventory(OtherActor)) Trigger(true);
			}
		}
	}
	else
	{
		// As long as Tagged-actor overlap (insert tagged-item / tagged-player walk on it)
		if (HasAcceptableTag(OtherActor)) 
		{
			ActivatorCount++;

			if (!bIsTriggered)
			{
				Trigger(true);
			}
		}
		
	}
}

void UTriggerBoxComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Lock || !OtherActor || OtherActor == GetOwner()) return;

	if (!Lock->GetIsRequiresKeyInventory())
	{
		// As long as Tagged-actor overlap (insert tagged-item / tagged-player walk on it)
		if (HasAcceptableTag(OtherActor)) 
		{
			ActivatorCount = FMath::Max(0, ActivatorCount - 1);

			if (ActivatorCount == 0 && bIsTriggered)
			{
				Trigger(false);
			}
		}
	}
}

// Check if the actor has any of the acceptable tags
bool UTriggerBoxComponent::HasAcceptableTag(AActor* ActorToCheck) const
{
	if (ActorToCheck)
	{
		for (const FName& ApprovedTag : AcceptableActorTags)
		{
			if (ActorToCheck->ActorHasTag(ApprovedTag))
			{
				return true;
			}
		}
	}

	return false;
}
