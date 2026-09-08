// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_CheckAttackRange();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Attack range threshold. Designers can tweak this directly on the node in the BT graph! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActor;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsAttackRange;

	
};
