// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventoryComponent::AddItem(UItemBase* NewItem)
{
	if (!NewItem) return false;

	// Storage full
	if (InventoryItems.Num() >= MaxInventorySize) return false;

	// If item stackable
	if (NewItem->NumericData.bIsStackable)
	{

		// Add item to existing stack

		for (UItemBase* ExistingItem : InventoryItems)
		{
			if (ExistingItem && ExistingItem->ItemID == NewItem->ItemID && !ExistingItem->IsFullItemStack())
			{
				int32 SpaceLeft = ExistingItem->NumericData.MaxStackSize - ExistingItem->Quantity;
				int32 QuantityToAdd = FMath::Min(NewItem->Quantity, SpaceLeft);
					
				// Update exsiting quantity & overloaded quantity
				ExistingItem->SetQuantity(ExistingItem->Quantity + QuantityToAdd);
				NewItem->SetQuantity(NewItem->Quantity - QuantityToAdd);

				if (NewItem->Quantity <= 0)
				{
					OnInventoryUpdated.Broadcast();
					return true; // All quantity added, no need to add new item
				}
			}
		}

		// Add remaining quantity as a new stack if there's space in the inventory
		if (NewItem->Quantity > 0 && InventoryItems.Num() < MaxInventorySize)
		{
			InventoryItems.Add(NewItem);
			OnInventoryUpdated.Broadcast();
		}
	}
	else
	{
		InventoryItems.Add(NewItem);
		OnInventoryUpdated.Broadcast();
	}

	// No error, item added successfully
	return true;
}

UItemBase* UInventoryComponent::FindItemInstanceByID(const FName TargetItemID) const
{
	if (!TargetItemID.IsNone())
	{
		for (UItemBase* ExistingItem : InventoryItems)
		{
			if (ExistingItem && ExistingItem->ItemID == TargetItemID)
			{
				return ExistingItem;
			}
		}
	}
	return nullptr;
}

bool UInventoryComponent::RemoveItemByInstance(UItemBase* ItemInstance, const int32 Quantity)
{
	if (ItemInstance && Quantity >= 1)
	{
		if (InventoryItems.Contains(ItemInstance))
		{
			if (ItemInstance->Quantity > Quantity) 
			{
				ItemInstance->SetQuantity(ItemInstance->Quantity - Quantity);
			}
			else
			{
				InventoryItems.RemoveSingle(ItemInstance);
			}

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Key removed.")));
			
			OnInventoryUpdated.Broadcast();
			return true;

		}
	}

	return false;
}

bool UInventoryComponent::HasQuestItem(UItemBase* ItemInstance, const int32 Quantity)
{
	if (ItemInstance && Quantity >= 1)
	{
		if (InventoryItems.Contains(ItemInstance))
		{
			if (ItemInstance->Quantity >= Quantity)
			{
				return true;
			}
		}
	}

	return false;
}

