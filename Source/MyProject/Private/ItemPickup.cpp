// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "ItemBase.h"
#include "InventoryComponent.h"
#include "PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidget.h"

// Sets default values
AItemPickup::AItemPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Create ItemMesh */

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	ItemMesh->SetSimulatePhysics(true);

	/* Widget Component */
	InteractPromptWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractPromptWidgetComponent"));
	InteractPromptWidgetComponent->SetupAttachment(RootComponent);
	InteractPromptWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractPromptWidgetComponent->SetVisibility(false);
}

void AItemPickup::ShowInteractionPrompt_Implementation()
{
	if (InteractPromptWidgetComponent)
	{
		InteractPromptWidgetComponent->SetVisibility(true);
	}
}

void AItemPickup::HideInteractionPrompt_Implementation()
{
	if (InteractPromptWidgetComponent)
	{
		InteractPromptWidgetComponent->SetVisibility(false);
	}
}

void AItemPickup::Interact_Implementation(APlayerCharacter* Character)
{
	if (Character && ItemInstance)
	{
		UInventoryComponent* CharacterInventory = Character->FindComponentByClass<UInventoryComponent>();

		if (CharacterInventory)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Pickup: Found Inventory Component! Sending item..."));

			UItemBase* ItemCopy = ItemInstance->CreateItemCopy();

			if (CharacterInventory->AddItem(ItemCopy))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Pickup: AddItem returned TRUE! Destroying actor."));
				Destroy();
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Inventory is full!"));
				if (ItemCopy)
				{
					ItemCopy->MarkAsGarbage();
				}
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Pickup Error: Character does NOT have a UInventoryComponent attached!"));
		}
	}
}

FInteractionData AItemPickup::GetInteractionData_Implementation() const
{
	FInteractionData Data;

	if (ItemInstance)
	{
		Data.InteractionText = ItemInstance->TextData.InteractionText;
		Data.DisplayName = ItemInstance->TextData.Name;
	}

	Data.InputAction = InteractionAction;

	return Data;
}

/* Item Pickup that spawned by events (Invisible in world at first) */
void AItemPickup::InitializePickup(const UDataTable* ItemDataTable, FName NewItemID)
{
	ProcessDataInitialization(ItemDataTable, NewItemID);
}

void AItemPickup::InitializeInteractionWidget()
{
	if (InteractPromptWidgetComponent)
	{
		UInteractionWidget* InteractionWidget = Cast<UInteractionWidget>(InteractPromptWidgetComponent->GetUserWidgetObject());
		
		if (InteractionWidget)
		{
			InteractionWidget->SetInteractionData(GetInteractionData_Implementation());
		}
	}
}

// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	/* Item Pickup spawned in world */
	if (!ItemRowHandle.IsNull() && ItemRowHandle.DataTable)
	{
		ProcessDataInitialization(ItemRowHandle.DataTable, ItemRowHandle.RowName);
	}

	InitializeInteractionWidget();
}

void AItemPickup::ProcessDataInitialization(const UDataTable* ItemDataTable, FName NewItemID)
{
	if (ItemDataTable && !NewItemID.IsNone())
	{
		ItemRowHandle.DataTable = ItemDataTable;
		ItemRowHandle.RowName = NewItemID;
		
		FItemData* RowData = ItemRowHandle.GetRow<FItemData>(TEXT("Pickup Initialization Context"));

		if (RowData)
		{
			ItemInstance = NewObject<UItemBase>(this, UItemBase::StaticClass());

			if (ItemInstance)
			{
				// Copy the data from the row/table to the item instance
				ItemInstance->ItemID = RowData->ItemID;
				ItemInstance->ItemType = RowData->ItemType;
				ItemInstance->ItemRarity = RowData->ItemRarity;
				ItemInstance->TextData = RowData->TextData;
				ItemInstance->NumericData = RowData->NumericData;
				ItemInstance->AssetData = RowData->AssetData;

				// Add ItemId to Tag just incase not specified
				Tags.AddUnique(ItemInstance->ItemID);

				// Default
				ItemInstance->Quantity = 1;

				// Set the mesh for the item pickup
				if (RowData->AssetData.Mesh && ItemMesh)
				{
					ItemMesh->SetStaticMesh(RowData->AssetData.Mesh);
					
					// Dynamically set the mesh to be visible and enable collision and physics simulation
					ItemMesh->SetVisibility(true);
					ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					ItemMesh->RecreatePhysicsState();
					ItemMesh->SetSimulatePhysics(true);

					if (ItemMesh->IsPhysicsStateCreated())
					{
						ItemMesh->WakeAllRigidBodies();
					}
				}

				InitializeInteractionWidget();
			}
		}
	}	
}