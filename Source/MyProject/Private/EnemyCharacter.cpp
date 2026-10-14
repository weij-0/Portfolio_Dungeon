// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Perception/AISense_Damage.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	/* Initialization Properties */
	EnemyCurrentHealth = EnemyMaxHealth;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemyCharacter::HandleOnHealthChanged);
		HealthComponent->OnDeath.AddUniqueDynamic(this, &AEnemyCharacter::HandleOnDeath);

		HealthComponent->InitializeHealth(EnemyMaxHealth, EnemyCurrentHealth);
	}
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CachedEnemyAIController = Cast<AEnemyAIController>(NewController);
}

// HealthComponent braodcast to OnHealthChanged
void AEnemyCharacter::HandleOnHealthChanged(float Percent, float CurrentHealth, float MaxHealth)
{
	if (!CachedEnemyAIController)
	{
		CachedEnemyAIController = Cast<AEnemyAIController>(GetController());
	}

	if (CachedEnemyAIController)
	{
		CachedEnemyAIController->SetBBCurrentHealth(CurrentHealth);
		CachedEnemyAIController->SetBBMaxHealth(MaxHealth);

		if (bHasPhaseTwo && Percent <= 0.5) CachedEnemyAIController->SetBBIsPhaseTwo(true);
		
	}
	
}

// HealthComponent broadcast to OnDeath
void AEnemyCharacter::HandleOnDeath(AActor* DamagedActor, AController* InstigatedBy)
{
	if (!bIsAlive) return;

	bIsAlive = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();

	UBrainComponent* BrainComponent = CachedEnemyAIController->GetBrainComponent();

	if (BrainComponent)
	{
		BrainComponent->StopLogic("Dead");
	}

	

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Get attacker
	AActor* Attacker = EventInstigator ? EventInstigator->GetPawn() : DamageCauser;

	// Report damage to AI Perception automatically
	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,                         // Damaged Actor (AI)
		Attacker,                     // Instigator (Player)
		ActualDamage,                 // Damage
		Attacker->GetActorLocation(), // Location
		GetActorLocation()            // Hit Location
	);

	return ActualDamage;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

