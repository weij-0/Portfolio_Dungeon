// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverAComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Lock.h"

// Sets default values for this component's properties
UMoverAComponent::UMoverAComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoverAComponent::BeginPlay()
{
	Super::BeginPlay();

	SetShouldMove(false);

	Lock = Cast<ALock>(GetOwner());

	if (Lock)
	{
		PrimaryMesh = Lock->GetPrimaryMesh();
		SecondaryMesh = Lock->GetSecondaryMesh();
	}
	
	/* Primary Mesh */

	if (PrimaryMesh)
	{
		// Start Location
		PrimaryStartLocation = PrimaryMesh->GetRelativeLocation();
		PrimaryStartRotation = PrimaryMesh->GetRelativeRotation();

		// Target Location
		PrimaryTargetLocation = PrimaryStartLocation;
		PrimaryTargetRotation = PrimaryStartRotation;
	}

	/* Secondary Mesh */

	if (SecondaryMesh)
	{
		// Start Location
		SecondaryStartLocation = SecondaryMesh->GetRelativeLocation();
		SecondaryStartRotation = SecondaryMesh->GetRelativeRotation();

		// Target Location
		SecondaryTargetLocation = SecondaryStartLocation;
		SecondaryTargetRotation = SecondaryStartRotation;
	}


	
}


// Called every frame
void UMoverAComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MoverMode == EMoverMode::Slide)
	{
		if (PrimaryMesh)
		{
			FVector NewLocation = MoveTranslation(PrimaryMesh->GetRelativeLocation(), PrimaryTargetLocation, PrimaryLinearOffset, DeltaTime, MoveTime);
			PrimaryMesh->SetRelativeLocation(NewLocation);
		}

		if (SecondaryMesh)
		{
			FVector NewLocation = MoveTranslation(SecondaryMesh->GetRelativeLocation(), SecondaryTargetLocation, SecondaryLinearOffset, DeltaTime, MoveTime);
			SecondaryMesh->SetRelativeLocation(NewLocation);
		}

	}
	else if (MoverMode == EMoverMode::Rotate)
	{
		if (PrimaryMesh)
		{
			FRotator NewRotation = MoveRotation(PrimaryMesh->GetRelativeRotation(), PrimaryTargetRotation, PrimaryRotationOffset, DeltaTime, MoveTime);
			PrimaryMesh->SetRelativeRotation(NewRotation);
		}

		if (SecondaryMesh)
		{
			FRotator NewRotation = MoveRotation(SecondaryMesh->GetRelativeRotation(), SecondaryTargetRotation, SecondaryRotationOffset, DeltaTime, MoveTime);
			SecondaryMesh->SetRelativeRotation(NewRotation);
		}

	}

}


/* Move Time */

float UMoverAComponent::GetMoveTime()
{
	return MoveTime;
}

void UMoverAComponent::SetMoveTime(float NewMoveTime)
{
	MoveTime = NewMoveTime;
}

/* Should Move */

bool UMoverAComponent::GetShouldMove()
{
	return bShouldMove;
}

void UMoverAComponent::SetShouldMove(bool bNewShouldMove)
{
	bShouldMove = bNewShouldMove;

	// Calculate and Set TargetLocation
	if (bShouldMove)
	{
		if (MoverMode == EMoverMode::Slide)
		{
			PrimaryTargetLocation = PrimaryStartLocation + PrimaryLinearOffset;
			SecondaryTargetLocation = SecondaryStartLocation + SecondaryLinearOffset;
		}
		else if (MoverMode == EMoverMode::Rotate)
		{
			PrimaryTargetRotation = PrimaryStartRotation + PrimaryRotationOffset;
			SecondaryTargetRotation = SecondaryStartRotation + SecondaryRotationOffset;
		}
	}
	else
	{
		if (MoverMode == EMoverMode::Slide)
		{
			PrimaryTargetLocation = PrimaryStartLocation;
			SecondaryTargetLocation = SecondaryStartLocation;
		}
		else if (MoverMode == EMoverMode::Rotate)
		{
			PrimaryTargetRotation = PrimaryStartRotation;
			SecondaryTargetRotation = SecondaryStartRotation;
		}
	}
}

FVector UMoverAComponent::MoveTranslation(FVector CurrentLocation, FVector TargetLocation, FVector LinearOffset, float DeltaTime, float InterpolationTime)
{
	if (!CurrentLocation.Equals(TargetLocation)) // if CurrentLocation not TargetLocation
	{
		float Speed = LinearOffset.Size() / InterpolationTime;

		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		
		return NewLocation;
	}

	return CurrentLocation;
}

FRotator UMoverAComponent::MoveRotation(FRotator CurrentRotation, FRotator TargetRotation, FRotator RotationOffset, float DeltaTime, float InterpolationTime)
{
	if (!CurrentRotation.Equals(TargetRotation)) // if CurrentLocation not TargetLocation
	{
		float Speed = FMath::Max3(FMath::Abs(RotationOffset.Pitch), FMath::Abs(RotationOffset.Yaw), FMath::Abs(RotationOffset.Roll)) / InterpolationTime;

		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);

		return NewRotation;
	}
	
	return CurrentRotation;
}


