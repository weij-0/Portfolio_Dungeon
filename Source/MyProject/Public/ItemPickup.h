// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "InteractionInterface.h"
#include "ItemPickup.generated.h"

class UStaticMeshComponent;
class UItemBase;
class APlayerCharacter;
class UDataTable;
class UInteractableComponent;

UCLASS()
class MYPROJECT_API AItemPickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AItemPickup();

	/* Functions */

	// Interaction Interface
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FInteractionData GetInteractionData_Implementation() const override;
	
	// Initialize the item pickup with data from a DataTable
	UFUNCTION(BlueprintCallable, Category = "Item Pickup")
	void InitializePickup(const UDataTable* ItemDataTable, FName NewItemID);

	/* Components & Properties */
	
	// InteractableComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInteractableComponent> InteractableComp;

	// Datas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickup|Interaction")
	TObjectPtr<const UInputAction> InteractionAction = nullptr;

	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Properties */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	UItemBase* ItemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	UStaticMeshComponent* ItemMesh;

	/* Functions */

	UFUNCTION(BlueprintCallable, Category = "Item Pickup")
	void ProcessDataInitialization(const UDataTable* ItemDataTable, FName NewItemID);

};
