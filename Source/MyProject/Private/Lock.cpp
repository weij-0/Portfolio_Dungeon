// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TriggerBoxComponent.h"
#include "TriggerInteractComponent.h"
#include "MoverAComponent.h"
#include "InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidget.h"

#include "Kismet/GameplayStatics.h"

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
	InteractPromptWidgetComponent = FindComponentByClass<UWidgetComponent>();

	if (TriggerBoxComp)
	{
		UE_LOG(LogTemp, Display, TEXT("Trigger Box Component found on %s"), *GetName());
	}

	if (TriggerInteractComp)
	{
		UE_LOG(LogTemp, Display, TEXT("Trigger Interact Component found on %s"), *GetName());
	}

	if (InteractPromptWidgetComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Trigger Interact Component found on %s"), *GetName());
		InteractPromptWidgetComponent->SetVisibility(false);
	}

	// Key Item Mesh Visibility
	if (KeyItemMesh)
	{
		KeyItemMesh->SetVisibility(bIsKeyPlaced);
	}

	InitializeInteractionWidget();
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALock::ShowInteractionPrompt_Implementation()
{
	if (InteractPromptWidgetComponent && !TriggerInteractComp->GetIsTriggered())
	{
		InteractPromptWidgetComponent->SetVisibility(true);
	}
}

void ALock::HideInteractionPrompt_Implementation()
{
	if (InteractPromptWidgetComponent)
	{
		InteractPromptWidgetComponent->SetVisibility(false);
	}
}

void ALock::Interact_Implementation(AActor* InteractorActor)
{
	if (TriggerInteractComp && InteractorActor)
	{
		TriggerInteractComp->Trigger(true, InteractorActor);

		// Hide prompt if the trigger succeeded
		if (TriggerInteractComp->GetIsTriggered())
		{
			HideInteractionPrompt_Implementation();
		}
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

void ALock::InitializeInteractionWidget()
{
	if (InteractPromptWidgetComponent)
	{
		UInteractionWidget* InteractionWidget = Cast<UInteractionWidget>(InteractPromptWidgetComponent->GetUserWidgetObject());

		if (!InteractionWidget)
		{
			// Force construction if Slate hasn't initialized the widget yet
			InteractPromptWidgetComponent->InitWidget();
			InteractionWidget = Cast<UInteractionWidget>(InteractPromptWidgetComponent->GetUserWidgetObject());
		}

		if (InteractionWidget)
		{
			InteractionWidget->SetInteractionData(GetInteractionData_Implementation());
		}
	}
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

