// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayAttackMontage.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UBTTask_PlayAttackMontage::UBTTask_PlayAttackMontage()
{
    NodeName = "Play Attack Montage";
}

EBTNodeResult::Type UBTTask_PlayAttackMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();

    ACharacter* Character = AIController ? Cast<ACharacter>(AIController->GetPawn()) : nullptr;

    if (!Character || !AttackMontage)
    {
        return EBTNodeResult::Failed;
    }

    UAnimInstance* AnimInstance = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;
    if (!AnimInstance)
    {
        return EBTNodeResult::Failed;
    }

    float Duration = Character->PlayAnimMontage(AttackMontage);
    if (Duration <= 0.0f)
    {
        return EBTNodeResult::Failed;
    }

    // Bind end delegate to notify BT when montage completes
    FOnMontageEnded EndDelegate; // Temporary delegate defined in "Animation/AnimInstance.h"
    EndDelegate.BindUObject(this, &UBTTask_PlayAttackMontage::OnMontageEnded, &OwnerComp);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

    // Task remains active in BT until Montage ends
    return EBTNodeResult::InProgress;
}

void UBTTask_PlayAttackMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
    if (OwnerComp)
    {
        EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
        FinishLatentTask(*OwnerComp, Result);
    }
}
