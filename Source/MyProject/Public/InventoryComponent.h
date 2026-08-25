// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "InventoryComponent.generated.h"

// Expose a delegate so your UI widgets can re-draw whenever items update
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemBase* NewItem);

	UItemBase* FindItemInstanceByID(const FName TargetItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByInstance(UItemBase* ItemInstance, const int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasQuestItem(UItemBase* ItemInstance, const int32 Quantity);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<UItemBase*> InventoryItems;

private:
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxInventorySize = 20;

};
