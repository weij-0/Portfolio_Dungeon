// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ComboWindow.h"
#include "ComboAttackComponent.h"

void UAnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		ComboComp = Owner->FindComponentByClass<UComboAttackComponent>();

		if (ComboComp)
		{
			ComboComp->OpenComboWindow();
		}
	}
	
}

void UAnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		ComboComp = Owner->FindComponentByClass<UComboAttackComponent>();

		if (ComboComp)
		{
			ComboComp->CloseComboWindow();
		}
	}
}
