// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "EnemyCharacter.h"


AEnemyAIController::AEnemyAIController()
{
	/* Initialize Blackboard Key Names */
	// Self
	SelfSpawnLocationKey.SelectedKeyName = "SelfSpawnLocation";
	CurrentHealthKey.SelectedKeyName = "CurrentHealth";
	MaxHealthKey.SelectedKeyName = "MaxHealth";

	// Target
	TargetActorKey.SelectedKeyName = "TargetActor";
	LastSeenTargetLocationKey.SelectedKeyName = "LastSeenTargetLocation";

	// Flags
	HasLineOfSightKey.SelectedKeyName = "HasLineOfSight";
	IsPhaseTwoKey.SelectedKeyName = "IsPhaseTwo";

	/* Create Perception Components */
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	if (PerceptionComp && SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = 1600.0f;
		SightConfig->PeripheralVisionAngleDegrees = 60.0f;
		SightConfig->SetMaxAge(5.0f);

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		PerceptionComp->ConfigureSense(*SightConfig);
		PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	if (PerceptionComp && DamageConfig)
	{
		DamageConfig->SetMaxAge(5.0f);
		PerceptionComp->ConfigureSense(*DamageConfig);
	}
	
}

void AEnemyAIController::SetBBCurrentHealth(float newCurrentHealth)
{
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsFloat(CurrentHealthKey.SelectedKeyName, newCurrentHealth);
	}
}

void AEnemyAIController::SetBBMaxHealth(float newMaxHealth)
{
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsFloat(MaxHealthKey.SelectedKeyName, newMaxHealth);
	}
}

void AEnemyAIController::SetBBIsPhaseTwo(bool bIsPhaseTwo)
{
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsBool(IsPhaseTwoKey.SelectedKeyName, bIsPhaseTwo);
	}
}

void AEnemyAIController::ClearTargetActor()
{
	// Clear Focus
	ClearFocus(EAIFocusPriority::Gameplay);

	// Clear Timer
	GetWorld()->GetTimerManager().ClearTimer(TargetForgetTimer);

	// Clear Blackboard values
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->ClearValue(TargetActorKey.SelectedKeyName);
		BB->ClearValue(LastSeenTargetLocationKey.SelectedKeyName);
		BB->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
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
				SelfSpawnLocationKey.ResolveSelectedKey(*BB->GetBlackboardAsset());
				CurrentHealthKey.ResolveSelectedKey(*BB->GetBlackboardAsset());
				MaxHealthKey.ResolveSelectedKey(*BB->GetBlackboardAsset());
				TargetActorKey.ResolveSelectedKey(*BB->GetBlackboardAsset());
				LastSeenTargetLocationKey.ResolveSelectedKey(*BB->GetBlackboardAsset());
				HasLineOfSightKey.ResolveSelectedKey(*BB->GetBlackboardAsset());
				IsPhaseTwoKey.ResolveSelectedKey(*BB->GetBlackboardAsset());

				/* Initialize blackboard values */
				BB->SetValueAsVector(SelfSpawnLocationKey.SelectedKeyName, InPawn->GetActorLocation());
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
	if (!Actor || !Actor->ActorHasTag(FName("Player")))	return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

											;
	UE_LOG(LogTemp, Warning, TEXT("[1] Sensed Signal From: %s"), *Actor->GetName());

	/* Case A: Damage Sense (when damage taken) */
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Damage Sensed]"));
		// Clear Timer (don't forget target)
		GetWorld()->GetTimerManager().ClearTimer(TargetForgetTimer);

		// Sensed target - Set target
		BB->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, true);
		
		BB->SetValueAsObject(TargetActorKey.SelectedKeyName, Actor);
		BB->SetValueAsVector(LastSeenTargetLocationKey.SelectedKeyName, Actor->GetActorLocation());

		return;
	}

	/* Case B: Sight Sense (when see target) */
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Sight Sensed]"));
		if (Stimulus.WasSuccessfullySensed())
		{
			// Clear Timer (don't forget target)
			GetWorld()->GetTimerManager().ClearTimer(TargetForgetTimer);

			// Found target - Set target
			BB->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, true);
			
			BB->SetValueAsObject(TargetActorKey.SelectedKeyName, Actor);
			BB->SetValueAsVector(LastSeenTargetLocationKey.SelectedKeyName, Actor->GetActorLocation());
		}
		else
		{
			// Store last seen location before starting timer
			BB->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
			
			BB->SetValueAsVector(LastSeenTargetLocationKey.SelectedKeyName, Stimulus.StimulusLocation);

			// Lost of sight - Clear target
			GetWorld()->GetTimerManager().SetTimer(
				TargetForgetTimer,
				this,
				&AEnemyAIController::ClearTargetActor,
				5.0f,
				false
			);
		}

		return;
	}
}
