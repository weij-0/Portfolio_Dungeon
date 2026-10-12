// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_SetFocus::UBTTask_SetFocus()
{
	NodeName = TEXT("Set Focus");
}

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!TargetActor) return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	/* Set Focus */
	AIController->SetFocus(TargetActor);

	return EBTNodeResult::Succeeded;

}
