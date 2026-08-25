// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TriggerBoxComponent.h"
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

	TriggerComp = CreateDefaultSubobject<UTriggerBoxComponent>(TEXT("TriggerComp"));
	TriggerComp->SetupAttachment(RootComp);

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

	if (KeyItemMesh)
	{
		KeyItemMesh->SetVisibility(bIsKeyPlaced);
	}
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("KeyItemID: %s"), *KeyItemID.ToString()));
			
			if (KeyItemInstance && InventoryComp->HasQuestItem(KeyItemInstance, KeyItemQty))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("HasQuestItem")));
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("KeyItemInstance: %s"), *KeyItemInstance->ItemID.ToString()));

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

