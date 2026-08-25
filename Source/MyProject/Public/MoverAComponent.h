// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoverAComponent.generated.h"

class UStaticMeshComponent;
class ALock;

UENUM(BlueprintType)
enum class EMoverMode : uint8
{
	Rotate   UMETA(DisplayName = "Rotate / Swing"),
	Slide    UMETA(DisplayName = "Slide / Translate")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMoverAComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoverAComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Functions */

	float GetMoveTime();
	void SetMoveTime(float NewMoveTime);

	bool GetShouldMove();
	void SetShouldMove(bool bNewShouldMove);

	// Core - moving the meshes
	FVector MoveTranslation(FVector CurrentLocation, FVector TargetLocation, FVector LinearOffset, float DeltaTime, float InterpolationTime);
	FRotator MoveRotation(FRotator CurrentRotation, FRotator TargetRotation, FRotator RotationOffset, float DeltaTime, float InterpolationTime);

protected:

	/* Properties */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Settings")
	EMoverMode MoverMode = EMoverMode::Rotate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Settings")
	float MoveTime = 4.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Settings")
	bool bShouldMove = false;

	// Primary

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Primary Movement Settings")
	FVector PrimaryLinearOffset = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Primary Movement Settings")
	FRotator PrimaryRotationOffset = FRotator(0.f, 90.f, 0.f);

	// Secondary

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Secondary Movement Settings")
	FVector SecondaryLinearOffset = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Secondary Movement Settings")
	FRotator SecondaryRotationOffset = FRotator(0.f, 90.f, 0.f);

private:

	/* Mesh */

	UPROPERTY()
	UStaticMeshComponent* PrimaryMesh = nullptr;

	UPROPERTY()
	UStaticMeshComponent* SecondaryMesh = nullptr;

	/*Initialization*/

	ALock* Lock;

	// Primary
	FVector PrimaryStartLocation;
	FVector PrimaryTargetLocation;
	FRotator PrimaryStartRotation; // Rotation
	FRotator PrimaryTargetRotation;

	// Secondary
	FVector SecondaryStartLocation;
	FVector SecondaryTargetLocation;
	FRotator SecondaryStartRotation; // Rotation
	FRotator SecondaryTargetRotation;
	
};
