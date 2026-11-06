// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractTraceComponent.h"
#include "InteractionInterface.h"
#include "InteractableComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

// Sets default values for this component's properties
UInteractTraceComponent::UInteractTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}


// Called when the game starts
void UInteractTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UInteractTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* Pawn = Cast<APawn>(GetOwner());

	if (!Pawn || !Pawn->IsLocallyControlled()) return;

	PerformInteractionTrace();
}

bool UInteractTraceComponent::TryInteract()
{
	UInteractableComponent* TargetInteractable = FocusedInteractable.Get();
	
	if (!IsValid(TargetInteractable)) return false;

	AActor* Interactor = GetOwner();

	if (!IInteractionInterface::Execute_CanInteract(TargetInteractable, Interactor)) return false;

	// Perform the interaction
	IInteractionInterface::Execute_Interact(TargetInteractable, Interactor);
	return true;
}

void UInteractTraceComponent::ClearFocus()
{
	SetFocus(nullptr);
}

void UInteractTraceComponent::SetFocus(UInteractableComponent* NewFocus)
{
	UInteractableComponent* OldFocus = FocusedInteractable.Get();

	if (NewFocus == OldFocus)
	{
		// Previous target was destroyed (e.g. picked up) and nothing replaced it: clean up once
		if (!NewFocus && !FocusedInteractable.IsExplicitlyNull())
		{
			FocusedInteractable = nullptr;
		}
		return;
	}

	AActor* Interactor = GetOwner();

	if (OldFocus)
	{
		IInteractionFocusListener::Execute_OnFocusLost(OldFocus, Interactor);
	}

	FocusedInteractable = NewFocus;

	if (NewFocus)
	{
		IInteractionFocusListener::Execute_OnFocusGained(NewFocus, Interactor);
	}
}

/* Interaction Trace */

void UInteractTraceComponent::PerformInteractionTrace()
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn || !Pawn->IsLocallyControlled()) return;

	const AMyPlayerController* PlayerController = Cast<AMyPlayerController>(Pawn->GetController());
	if (!PlayerController || !PlayerController->PlayerCameraManager) return;

	// Pawn location and forward vector
	const FVector PawnLocation = Pawn->GetActorLocation();
	const FVector PawnForwardVector = Pawn->GetActorForwardVector();

	// Camera location and forward vector
	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector CameraForwardVector = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();

	// Trace parameters
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Pawn);

	/* TRACE */
	GetWorld()->OverlapMultiByChannel(OverlapResults, PawnLocation, FQuat::Identity,
		TraceChannel, FCollisionShape::MakeSphere(InteractionTraceDistance), TraceParams);

	AActor* NearestActor = nullptr;
	float NearestDistance = InteractionTraceDistance;

	for (const FOverlapResult& Overlap : OverlapResults)
	{
		AActor* Actor = Overlap.GetActor();
		
		UInteractableComponent* InteractableComp = Actor->FindComponentByClass<UInteractableComponent>();
		if (!InteractableComp) continue;

		/* Calculate score */

		const float Distance = CalculateInteractionScore(InteractableComp, PawnLocation, CameraLocation, CameraForwardVector);
		
		/* Nearest Actor */	
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestActor = Actor;
		}
	}

	// Set focus to the highest scoring interactable
	if (NearestActor)
	{
		UInteractableComponent* NewFocus = NearestActor->FindComponentByClass<UInteractableComponent>();
		SetFocus(NewFocus);
	}
	else
	{
		SetFocus(nullptr);
	}
}

/* Calculation */

float UInteractTraceComponent::CalculateInteractionScore(const UInteractableComponent* Candidate, const FVector& PawnLoc,
	const FVector& CamLoc, const FVector& CamDir) const
{
	const FVector TargetLoc = Candidate->GetComponentLocation();
	const float Dist = FVector::Dist(PawnLoc, TargetLoc);

	return Dist;
}
