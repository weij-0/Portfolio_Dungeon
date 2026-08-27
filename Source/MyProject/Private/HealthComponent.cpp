// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* MyOwner = GetOwner())
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
}

void UHealthComponent::InitializeHealth(float InitMaxHealth, float InitCurrentHealth)
{
	MaxHealth = InitMaxHealth;
	CurrentHealth = FMath::Clamp(InitCurrentHealth, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(GetCurrentHealthPercent());
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage > 0.0f && CurrentHealth > 0.0f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

		float Percent = (CurrentHealth > 0.0f) ? CurrentHealth / MaxHealth : 0.0f;

		OnHealthChanged.Broadcast(Percent);

		if (CurrentHealth <= 0.0f)
		{
			OnDeath.Broadcast(InstigatedBy);
		}
	}

}

