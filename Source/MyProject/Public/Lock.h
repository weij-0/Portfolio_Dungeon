// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lock.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTriggerBoxComponent;
class UMoverAComponent;

UCLASS()
class MYPROJECT_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Functions */

	FORCEINLINE UStaticMeshComponent* GetPrimaryMesh() const { return PrimaryMesh; }
	FORCEINLINE UStaticMeshComponent* GetSecondaryMesh() const { return SecondaryMesh; }

	bool ProcessLockCheckInventory(AActor* ActorToCheck);

	bool GetIsRequiresKeyInventory();
	void SetIsRequiresKeyInventory(bool NewIsRequiresKeyInventory);

	bool GetIsKeyPlaced();
	void SetIsKeyPlaced(bool NewIsKeyPlaced);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* PrimaryMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* SecondaryMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* KeyItemMesh;

	/* Components */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTriggerBoxComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMoverAComponent* MoverComp;

	/* Properties */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Keys and Tags")
	FName KeyItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Keys and Tags")
	int32 KeyItemQty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Keys and Tags")
	bool bIsRequiresKeyInventory = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Keys and Tags")
	bool bRemoveKeyOnUse = false;

private:

	UPROPERTY(VisibleAnywhere, Category = Category = "Keys and Tags")
	bool bIsKeyPlaced = false;
};
