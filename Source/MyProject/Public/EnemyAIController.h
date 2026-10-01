// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UBlackBoard;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	/* Functions */
	/* Setter & Getter */

	// Used in EnemyCharacter.cpp
	void SetBBCurrentHealth(float newCurrentHealth);
	void SetBBMaxHealth(float newMaxHealth);
	void SetBBIsPhaseTwo(bool bIsPhaseTwo);
		
	// Set Target
	void ClearTargetActor();

	// Get TeamID
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }


protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	/* Functions */
	UFUNCTION()
	void  OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/* Components */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAISenseConfig_Damage* DamageConfig;

	/** Set Team ID: 1 for Enemies/Bosses, 0 for Player */
	FGenericTeamId TeamID = FGenericTeamId(1);

	/* Blackboard Key Name */
	// Self
	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector SelfSpawnLocationKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector CurrentHealthKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector MaxHealthKey;

	// Target
	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector LastSeenTargetLocationKey;

	// Flags
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FBlackboardKeySelector HasLineOfSightKey;

	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector IsAttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector IsPhaseTwoKey;

	/* Timer */
	FTimerHandle TargetForgetTimer;

	


private:


};
