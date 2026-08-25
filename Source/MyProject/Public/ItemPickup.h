// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemPickup.generated.h"

class UStaticMeshComponent;
class UItemBase;
class APlayerCharacter;
class UWidgetComponent;
class UDataTable;

UCLASS()
class MYPROJECT_API AItemPickup : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AItemPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/* Properties */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	UWidgetComponent* InteractPromptWidgetComponent;

	/* Functions */

	UFUNCTION(BlueprintCallable, Category = "Item Pickup")
	void InitializePickup(const UDataTable* ItemDataTable, FName NewItemID);

	
	UFUNCTION(BlueprintCallable, Category = "Item Pickup")
	void TriggerPickup(APlayerCharacter* Character);

protected:

	/* Properties */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	UItemBase* ItemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickup")
	UStaticMeshComponent* ItemMesh;
	
	/* Functions */

	UFUNCTION(BlueprintCallable, Category = "Item Pickup")
	void ProcessDataInitialization(const UDataTable* ItemDataTable, FName NewItemID);

};
