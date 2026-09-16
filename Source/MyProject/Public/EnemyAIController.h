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
/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	/* Setter & Getter */
	// Self
	UFUNCTION(BlueprintCallable, Category = "AI|BlackBoard")
	void SetBBSelfSpawnLocation(FVector NewSelfSpawnLocation);

	UFUNCTION(BlueprintAuthorityOnly, Category = "AI|BlackBoard")
	void SetBBSelfSpawnRotator(FRotator NewSelfSpawnRotator);

	UFUNCTION(BlueprintCallable, Category = "AI|BlackBoard")
	void SetBBSelfLocation(FVector NewSelfLocation);

	UFUNCTION(BlueprintAuthorityOnly, Category = "AI|BlackBoard")
	void SetBBSelfRotator(FRotator NewSelfRotator);

	UFUNCTION(BlueprintAuthorityOnly, Category = "AI|BlackBoard")
	void SetBBCurrentHealth(float NewCurrentHealth);

	UFUNCTION(BlueprintAuthorityOnly, Category = "AI|BlackBoard")
	void SetBBMaxHealth(float NewMaxHealth);

	// Target
	UFUNCTION(BlueprintCallable, Category = "AI|BlackBoard")
	void SetBBTargetActor(AActor* NewTargetActor);

	UFUNCTION(BlueprintCallable, Category = "AI|BlackBoard")
	void SetBBLastSeenTargetLocation(FVector NewLastSeenTargetLocation);

	// Flags
	UFUNCTION(BlueprintCallable, Category = "AI|BlackBoard")
	void SetBBIsAlerted(bool bIsAlerted);

	UFUNCTION(BlueprintCallable, Category = "AI|BlackBoard")
	void SetBBIsAttackRange(bool bIsAttackRange);

	

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|BlackBoard")
	UBlackboardComponent* CachedBB;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAISenseConfig_Sight* SightConfig;


	/* Blackboard Key Name */
	// Self
	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector SelfSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector SelfSpawnRotator;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector SelfLocation;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector SelfRotator;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BlackBoard")
	FBlackboardKeySelector MaxHealth;

	// Target
	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector TargetActor;

	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector LastSeenTargetLocation;

	// Flags
	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector IsAlerted;

	UPROPERTY(EditAnywhere, Category = "AI|BlackBoard")
	FBlackboardKeySelector IsAttackRange;


	/* Functions */
	UFUNCTION()
	void  OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);


private:


};
