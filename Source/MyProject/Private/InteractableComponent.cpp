// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"
#include "InteractionWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

/* IInteractionInterface */

bool UInteractableComponent::CanInteract_Implementation(AActor* Interactor) const
{
	AActor* Owner = GetOwner();

	if (Owner && Owner->Implements<UInteractionInterface>())
	{
		return IInteractionInterface::Execute_CanInteract(Owner, Interactor);
	}

	return bInteractionEnabled;
}

void UInteractableComponent::Interact_Implementation(AActor* Interactor)
{
	AActor* Owner = GetOwner();

	if (Owner && Owner->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_Interact(Owner, Interactor);
	}

	RefreshPrompt();

	// The owner may destroyed (e.g. a picked-up item)
	if (!IsValid(this) || !IsValid(GetOwner())) return;

	OnInteract.Broadcast(Interactor);
}

FInteractionData UInteractableComponent::GetInteractionData_Implementation() const
{
	AActor* Owner = GetOwner();

	if (Owner && Owner->Implements<UInteractionInterface>())
	{
		return IInteractionInterface::Execute_GetInteractionData(Owner);
	}

	return Data;
}

/* OnFocusGained / Lost */

void UInteractableComponent::OnFocusGained_Implementation(AActor* Interactor)
{
	AActor* Owner = GetOwner();

	if (Owner && Owner->Implements<UInteractionFocusListener>())
	{
		return IInteractionFocusListener::Execute_OnFocusGained(Owner, Interactor);
	}

	CurrentInteractor = Interactor;
	bIsFocused = true;
	
	RefreshPrompt();
}

void UInteractableComponent::OnFocusLost_Implementation(AActor* Interactor)
{
	AActor* Owner = GetOwner();

	if (Owner && Owner->Implements<UInteractionFocusListener>())
	{
		return IInteractionFocusListener::Execute_OnFocusLost(Owner, Interactor);
	}

	CurrentInteractor.Reset();
	bIsFocused = false;
	
	SetPromptVisible(false);
}

void UInteractableComponent::RefreshPrompt()
{
	if (!bIsFocused) return;

	AActor* Interactor = CurrentInteractor.Get();

	if (!IInteractionInterface::Execute_CanInteract(this, Interactor))
	{
		SetPromptVisible(false);
		return;
	}

	InitializePromptWidget();
	if (!PromptWidgetComponent) return;

	if (UInteractionWidget* PromptWidget = Cast<UInteractionWidget>(PromptWidgetComponent->GetUserWidgetObject()))
	{
		PromptWidget->SetInteractionData(IInteractionInterface::Execute_GetInteractionData(this));
	}

	SetPromptVisible(true);
}

void UInteractableComponent::SetInteractionEnabled(bool bNewEnabled)
{
	bInteractionEnabled = bNewEnabled;
	RefreshPrompt();
}

/* Setter / Getter */



void UInteractableComponent::SetPromptVisible(bool bShowPrompt)
{
	if (PromptWidgetComponent)
	{
		PromptWidgetComponent->SetVisibility(bShowPrompt);
	}
}

/* Initialize Prompt Widget*/

void UInteractableComponent::InitializePromptWidget()
{
	if (PromptWidgetComponent || !PromptWidgetClass) return;

	AActor* Owner = GetOwner();
	
	if (!Owner) return;

	PromptWidgetComponent = NewObject<UWidgetComponent>(Owner, TEXT("InteractionPrompt"));
	PromptWidgetComponent->SetWidgetClass(PromptWidgetClass);
	PromptWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PromptWidgetComponent->SetDrawAtDesiredSize(true);
	PromptWidgetComponent->SetVisibility(false);
	PromptWidgetComponent->RegisterComponent();
	PromptWidgetComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PromptWidgetComponent->InitWidget();
}