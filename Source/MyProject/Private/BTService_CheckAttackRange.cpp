// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");

	TargetActor.SelectedKeyName = "TargetActor";
	IsAttackRange.SelectedKeyName = "IsAttackRange";
	EnemyDistanceToTarget.SelectedKeyName = "EnemyDistanceToTarget";
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (BB && AIController)
	{
		AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActor.SelectedKeyName));
		
		if (Target)
		{
			float DistanceToTarget = FVector::Dist(AIController->GetPawn()->GetActorLocation(), Target->GetActorLocation());

			bool bIsInAttackRange = DistanceToTarget <= AttackRange;

			// Set the blackboard value for IsAttackRange
			BB->SetValueAsBool(IsAttackRange.SelectedKeyName, bIsInAttackRange);
			
			// Set the blackboard value for DistanceToTarget
			BB->SetValueAsFloat(EnemyDistanceToTarget.SelectedKeyName, DistanceToTarget);
		}
	}
}