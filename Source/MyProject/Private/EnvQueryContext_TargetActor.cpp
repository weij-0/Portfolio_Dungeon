// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_TargetActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{

	AActor* Owner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!Owner) return;

	// Get AI controller
	AAIController* AIController = nullptr;

	if (APawn* Pawn = Cast<APawn>(Owner))
	{
		AIController = Pawn->GetController<AAIController>();
	}
	else if (AAIController* Controller = Cast<AAIController>(Owner))
	{
		AIController = Controller;
	}

	if (AIController)
	{
		// Get BB component
		UBlackboardComponent* BB = AIController->GetBlackboardComponent();

		if (BB)
		{
			// Get the target actor from the blackboard
			AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		
			if (TargetActor)
			{
				// Set target actor as context data for the EQS query
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
			}
		}
	}

	}
