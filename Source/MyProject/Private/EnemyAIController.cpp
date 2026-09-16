// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyCharacter.h"


AEnemyAIController::AEnemyAIController()
{
	/* Initialize Blackboard Key Names */
	// Self
	SelfSpawnLocation.SelectedKeyName = "SelfSpawnLocation";
	SelfSpawnRotator.SelectedKeyName = "SelfSpawnRotator";
	SelfLocation.SelectedKeyName = "SelfLocation";
	SelfRotator.SelectedKeyName = "SelfRotator";
	CurrentHealth.SelectedKeyName = "CurrentHealth";
	MaxHealth.SelectedKeyName = "MaxHealth";

	// Target
	TargetActor.SelectedKeyName = "TargetActor";
	LastSeenTargetLocation.SelectedKeyName = "LastSeenTargetLocation";
	// Flags
	IsAlerted.SelectedKeyName = "IsAlerted";
	IsAttackRange.SelectedKeyName = "IsAttackRange";

	/* Create Perception Components */
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = 1600.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		if (PerceptionComp)
		{
			PerceptionComp->ConfigureSense(*SightConfig);
			PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] PerceptionComp failed to create in Constructor!"), *GetName());
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] SightConfig failed to create in Constructor!"), *GetName());
	}
}

void AEnemyAIController::SetBBSelfSpawnLocation(FVector NewSelfSpawnLocation)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsVector(SelfSpawnLocation.SelectedKeyName, NewSelfSpawnLocation);
	}
}

void AEnemyAIController::SetBBSelfSpawnRotator(FRotator NewSelfSpawnRotator)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsRotator(SelfSpawnRotator.SelectedKeyName, NewSelfSpawnRotator);
	}
}

void AEnemyAIController::SetBBSelfLocation(FVector NewSelfLocation)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsVector(SelfLocation.SelectedKeyName, NewSelfLocation);
	}
}

void AEnemyAIController::SetBBSelfRotator(FRotator NewSelfRotator)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsRotator(SelfRotator.SelectedKeyName, NewSelfRotator);
	}
}

void AEnemyAIController::SetBBCurrentHealth(float NewCurrentHealth)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsFloat(CurrentHealth.SelectedKeyName, NewCurrentHealth);
	}
}

void AEnemyAIController::SetBBMaxHealth(float NewMaxHealth)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsFloat(MaxHealth.SelectedKeyName, NewMaxHealth);
	}
}

void AEnemyAIController::SetBBTargetActor(AActor* NewTargetActor)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsObject(TargetActor.SelectedKeyName, NewTargetActor);
	}
}

void AEnemyAIController::SetBBLastSeenTargetLocation(FVector NewLastSeenTargetLocation)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsVector(LastSeenTargetLocation.SelectedKeyName, NewLastSeenTargetLocation);
	}
}

void AEnemyAIController::SetBBIsAlerted(bool bIsAlerted)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsBool(IsAlerted.SelectedKeyName, bIsAlerted);
	}
}

void AEnemyAIController::SetBBIsAttackRange(bool bIsAttackRange)
{
	UBlackboardComponent* BB = GetBlackboardComponent();

	if (BB)
	{
		BB->SetValueAsBool(IsAttackRange.SelectedKeyName, bIsAttackRange);
	}
}



void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (PerceptionComp)
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] BeginPlay: PerceptionComp is nullptr!"), *GetName());
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] OnPossess: InPawn is nullptr!"), *GetName());
		return;
	}

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);

	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to Cast EnemyCharacter!"), *GetName());
		return;
	}

	if (EnemyCharacter->BehaviorTreeAsset)
	{
		if (RunBehaviorTree(EnemyCharacter->BehaviorTreeAsset))
		{		
			if (UBlackboardComponent* BB = GetBlackboardComponent())
			{
				SelfSpawnLocation.ResolveSelectedKey(*BB->GetBlackboardAsset());
				SelfSpawnRotator.ResolveSelectedKey(*BB->GetBlackboardAsset());
				SelfLocation.ResolveSelectedKey(*BB->GetBlackboardAsset());
				SelfRotator.ResolveSelectedKey(*BB->GetBlackboardAsset());
				CurrentHealth.ResolveSelectedKey(*BB->GetBlackboardAsset());
				MaxHealth.ResolveSelectedKey(*BB->GetBlackboardAsset());
				TargetActor.ResolveSelectedKey(*BB->GetBlackboardAsset());
				LastSeenTargetLocation.ResolveSelectedKey(*BB->GetBlackboardAsset());
				IsAlerted.ResolveSelectedKey(*BB->GetBlackboardAsset());
				IsAttackRange.ResolveSelectedKey(*BB->GetBlackboardAsset());

				/* Initialize blackboard values */
				SetBBSelfSpawnLocation(InPawn->GetActorLocation());
				SetBBSelfSpawnRotator(InPawn->GetActorRotation());
			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to run AI: BehaviorTreeAsset failed to run!"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to run AI: No BehaviorTreeAsset assigned!"), *GetName());
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor && Actor->ActorHasTag(FName("Player")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 3.0f, FColor::Green,
					FString::Printf(TEXT("AI SPOTTED TARGET: %s"), *Actor->GetName())
				);
			}
			// Found target - Set target
			SetBBTargetActor(Actor);
			SetBBLastSeenTargetLocation(Actor->GetActorLocation());
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 3.0f, FColor::Red,
					TEXT("AI LOST TARGET")
				);
			}
			// Lost of sight - Clear target
			SetBBTargetActor(nullptr);
		}
	}
}
