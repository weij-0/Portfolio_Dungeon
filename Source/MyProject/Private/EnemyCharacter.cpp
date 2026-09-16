// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Health Component */
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	/* Initialization Properties */
	EnemyCurrentHealth = EnemyMaxHealth;

}

void AEnemyCharacter::ExecuteMeleeHit(FName SocketName, float Damage, float Radius)
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance() || SocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecuteMeleeHit: Invalid parameters or missing components."));
		return;
	}

	// Set up the collision shape for the trace
	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);

	FCollisionShape HitShape = FCollisionShape::MakeSphere(Radius);

	DrawDebugSphere(GetWorld(), SocketLocation, HitShape.GetSphereRadius(), 12, FColor::Red, false, 1.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self

	TArray<FHitResult> HitResults;

	// Perform Multi-Sphere Trace for Pawn channel
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SocketLocation,
		SocketLocation + (GetActorForwardVector() * 1.0f),
		FQuat::Identity,
		ECC_Pawn, // Detect Player Pawn
		HitShape,
		QueryParams
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this)
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					Damage,
					GetController(),
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

void AEnemyCharacter::ExecuteMeleeTrace(FName BaseSocketName, FName TipSocketName, float Damage, float Radius)
{
	if(!GetMesh() || !GetMesh()->GetAnimInstance() || BaseSocketName.IsNone() || TipSocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecuteMeleeTrace: Invalid parameters or missing components."));
		return;
	}

	FVector BaseLocation = GetMesh()->GetSocketLocation(BaseSocketName);
	FVector TipLocation = GetMesh()->GetSocketLocation(TipSocketName);

	// Initialize the last locations and exit
	if (!bHasLastLocation)
	{
		LastBaseLocation = BaseLocation;
		LastTipLocation = TipLocation;
		bHasLastLocation = true;
		return;
	}

	// Set up the collision shape for the trace
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self
	QueryParams.AddIgnoredActors(HitIgnoreList); // Ignore previously hit actors

	const int32 TraceSubdivisions = 4; // Number of subdivisions for the trace

	FCollisionShape HitShape = FCollisionShape::MakeSphere(Radius);

	for (int32 i = 0; i < TraceSubdivisions; i++)
	{
		float Alpha = (float)i / (float)TraceSubdivisions;

		FVector PreviousPointOnBlade = FMath::Lerp(LastBaseLocation, LastTipLocation, Alpha);
		FVector CurrentPointOnBlade = FMath::Lerp(BaseLocation, TipLocation, Alpha);
	
		TArray<FHitResult> HitResults;

		// Perform Multi-Sphere Trace for Pawn channel
		bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		PreviousPointOnBlade,
		CurrentPointOnBlade,
		FQuat::Identity,
		ECC_Pawn, // Detect Player Pawn
		HitShape,
		QueryParams
	);

#if WITH_EDITOR
	// Visualize the blade arc motion in editor
		DrawDebugLine(GetWorld(), PreviousPointOnBlade, CurrentPointOnBlade, FColor::Red, false, 1.0f);
		DrawDebugSphere(GetWorld(), CurrentPointOnBlade, Radius, 8, FColor::Green, false, 1.0f);
#endif

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this && !HitIgnoreList.Contains(HitActor))
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					Damage,
					GetController(),
					this,
					UDamageType::StaticClass()
				);
				HitIgnoreList.Add(HitActor);
			}
		}
	}

	LastBaseLocation = BaseLocation;
	LastTipLocation = TipLocation;
	
	}
}

void AEnemyCharacter::ClearHitIgnoreList()
{
	HitIgnoreList.Empty();
	bHasLastLocation = false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemyCharacter::HandleOnHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::HandleOnDeath);

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
	}
	
}

// HealthComponent braodcast to OnDeath
void AEnemyCharacter::HandleOnDeath(AController* InstigatedBy)
{
	UBrainComponent* BrainComponent = CachedEnemyAIController->GetBrainComponent();

	if (BrainComponent)
	{
		BrainComponent->StopLogic("Dead");
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

