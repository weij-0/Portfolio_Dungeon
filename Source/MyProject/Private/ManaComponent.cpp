// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UManaComponent::UManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UManaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* MyOwner = GetOwner())
	{
		//Pending MyOwner->OnSkillCasted.AddDynamic(this, &UManaComponent::HandleSkillCasted);
	}
	
}

void UManaComponent::InitializeMana(float InitMaxMana, float InitCurrentMana)
{
	MaxMana = InitMaxMana;
	CurrentMana = FMath::Clamp(InitCurrentMana, 0.0f, MaxMana);

	OnManaChanged.Broadcast(GetCurrentManaPercent(), GetCurrentMana(), GetMaxMana());
}

void UManaComponent::HandleSkillCasted(float ManaCost)
{
	if (ManaCost <= 0.0f || CurrentMana <= 0.0f)
	{
		return;
	}

	CurrentMana = FMath::Clamp(CurrentMana - ManaCost, 0.0f, MaxMana);

	OnManaChanged.Broadcast(GetCurrentManaPercent(), GetCurrentMana(), GetMaxMana());

}

