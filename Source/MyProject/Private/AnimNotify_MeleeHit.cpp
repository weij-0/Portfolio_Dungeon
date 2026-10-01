// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MeleeHit.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyCharacter.h"

void UAnimNotify_MeleeHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());

		if (Enemy)
		{
			Enemy->ExecuteMeleeHit(SocketName, Damage, HitRadius);
		}
		
	}
}
