// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Health Component */
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	/* Initialization Properties */
	EnemyCurrentHealth = EnemyMaxHealth;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CachedEnemyAIController = Cast<AEnemyAIController>(NewController);

	if (HealthComponent)
	{
		EnemyCurrentHealth = EnemyMaxHealth;
		HealthComponent->InitializeHealth(EnemyMaxHealth, EnemyCurrentHealth);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

