// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");

	TargetActorKey.SelectedKeyName = "TargetActor";
	TargetActorLocationKey.SelectedKeyName = "TargetActorLocation";
	EnemyDistanceToTargetKey.SelectedKeyName = "EnemyDistanceToTarget";

}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BB || !AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
			
	if (!AIPawn || !Target) return;

	/* Calculate Distance Between */

	float CenterDistance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());

	float SelfRadius = 0.0f;
	float TargetRadius = 0.0f;

	if (ACharacter* SelfChar = Cast<ACharacter>(AIPawn))
	{
		if (UCapsuleComponent* Capsule = SelfChar->GetCapsuleComponent())
		{
			SelfRadius = Capsule->GetScaledCapsuleRadius();
		}
	}

	if (ACharacter* TargetChar = Cast<ACharacter>(Target))
	{
		if (UCapsuleComponent* Capsule = TargetChar->GetCapsuleComponent())
		{
			TargetRadius = Capsule->GetScaledCapsuleRadius();
		}
	}

	float SurfaceDistance = FMath::Max(0.0f, CenterDistance - SelfRadius - TargetRadius);

	// Set the blackboard value
	BB->SetValueAsVector(TargetActorLocationKey.SelectedKeyName, Target->GetActorLocation());
	BB->SetValueAsFloat(EnemyDistanceToTargetKey.SelectedKeyName, SurfaceDistance);

	
}