// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TriggerBoxComponent.h"
#include "TriggerInteractComponent.h"
#include "MoverAComponent.h"
#include "InventoryComponent.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Root & Mesh */

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	PrimaryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimaryMesh"));
	PrimaryMesh->SetupAttachment(RootComp);

	SecondaryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryMesh"));
	SecondaryMesh->SetupAttachment(RootComp);

	KeyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Item Mesh"));
	KeyItemMesh->SetupAttachment(RootComp);

	/* Components */

	MoverComp = CreateDefaultSubobject<UMoverAComponent>(TEXT("MoverComp"));

	/* Properties */

	Tags.Add("Lock");
	KeyItemID = FName("DefaultKeyID");
	KeyItemQty = 1;
	bIsRequiresKeyInventory = false;
	bRemoveKeyOnUse = false;
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();

	// Dynamic Components
	TriggerBoxComp = FindComponentByClass<UTriggerBoxComponent>();
	TriggerInteractComp = FindComponentByClass<UTriggerInteractComponent>();

	// Key Item Mesh Visibility
	if (KeyItemMesh)
	{
		KeyItemMesh->SetVisibility(bIsKeyPlaced);
	}
}

bool ALock::CanInteract_Implementation(AActor* Interactor) const
{
	return TriggerInteractComp && !TriggerInteractComp->GetIsTriggered();
}

void ALock::Interact_Implementation(AActor* InteractorActor)
{
	if (TriggerInteractComp && InteractorActor)
	{
		TriggerInteractComp->Trigger(true, InteractorActor);
	}
}

FInteractionData ALock::GetInteractionData_Implementation() const
{
	FInteractionData Data;

	Data.InteractionText = FText::FromString("Open");
	//Data.DisplayName = FText::FromString("");
	Data.InputAction = InteractionAction;

	return Data;
}

bool ALock::ProcessLockCheckInventory(AActor* ActorToCheck)
{
	if (GetIsKeyPlaced()) return false;
	
	if (ActorToCheck)
	{
		UInventoryComponent* InventoryComp = ActorToCheck->FindComponentByClass<UInventoryComponent>();

		if (InventoryComp)
		{
			UItemBase* KeyItemInstance = InventoryComp->FindItemInstanceByID(KeyItemID);
			
			if (KeyItemInstance && InventoryComp->HasQuestItem(KeyItemInstance, KeyItemQty))
			{
				SetIsKeyPlaced(true);

				if (bRemoveKeyOnUse) InventoryComp->RemoveItemByInstance(KeyItemInstance, KeyItemQty);

				return true;
				
			}
		}
	}

	return false;
}

// IsRequiresKeyInventory
bool ALock::GetIsRequiresKeyInventory()
{
	return bIsRequiresKeyInventory;
}

void ALock::SetIsRequiresKeyInventory(bool NewIsRequiresKeyInventory)
{
	bIsRequiresKeyInventory = NewIsRequiresKeyInventory;
}

// IsKeyPlaced
bool ALock::GetIsKeyPlaced()
{
	return bIsKeyPlaced;
}

void ALock::SetIsKeyPlaced(bool NewIsKeyPlaced)
{
	bIsKeyPlaced = NewIsKeyPlaced;

	if (KeyItemMesh)
	{
		KeyItemMesh->SetVisibility(NewIsKeyPlaced);
	}
	
}

