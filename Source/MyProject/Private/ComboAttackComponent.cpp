// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAttackComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UComboAttackComponent::UComboAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UComboAttackComponent::OpenComboWindow()
{
	bComboWindowOpen = true;

	if (bNextAttackQueued)
	{
		PlayMontage(ComboIndex + 1);
	}
}

void UComboAttackComponent::CloseComboWindow()
{
	bComboWindowOpen = false;
}

void UComboAttackComponent::CancelCombo()
{
	if (!bIsAttacking) return;

	if (UAnimInstance* AnimInstance = GetOwnerAnimInstance())
	{
		// The end delegate resets the state and broadcasts OnComboFinished
		AnimInstance->Montage_Stop(0.2f, CurrentMontage);
	}
	else
	{
		ResetCombo();
		OnComboEnded.Broadcast(true);
	}
}

bool UComboAttackComponent::RequestAttack()
{

	if (ComboMontages.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Attack Failed: 'ComboMontages' array is empty! Assign montages in BP details."));
		return false;
	}
	
	/* 1st attack */
	if (!bIsAttacking)
	{
		ComboIndex = 0;
		
		return PlayMontage(ComboIndex);
	}

	/* 2nd attack or later */
	if (ComboIndex >= ComboMontages.Num() - 1)
	{
		return false; // if last combo, don't queue next attack
	}

	/* if ComboWindow is open = play, if not = queue */
	bNextAttackQueued = true;

	// Do next combo
	if (bComboWindowOpen)
	{
		return PlayMontage(ComboIndex + 1);
	}
	
	return true;
}

bool UComboAttackComponent::PlayMontage(int32 Index)
{
	// Record played montage
	UAnimMontage* PreviousMontage = CurrentMontage;

	// Current play montage
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	CurrentMontage = ComboMontages[Index];

	if (!AnimInstance || !CurrentMontage)
	{
		return false;
	}

	// If play failed
	if (AnimInstance->Montage_Play(CurrentMontage) <= 0.0f)
	{
		// rollback previous montage
		CurrentMontage = PreviousMontage;
		return false;
	}

	// If play successful
	bIsAttacking = true;
	bNextAttackQueued = false;
	bComboWindowOpen = false;
	ComboIndex = Index;

	/* Holds the function and only calls the function when the montage ends */
	FOnMontageEnded OnMontageEndedDelegate;
	OnMontageEndedDelegate.BindUObject(this, &UComboAttackComponent::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, CurrentMontage);

	// For AI use
	OnComboStepChanged.Broadcast(ComboIndex);


	return true;
}

void UComboAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Keep the combo alive, e.g. if combo_1(ended) != combo_2(started), don't reset combo.
	if (Montage != CurrentMontage) return;

	ResetCombo();
	OnComboEnded.Broadcast(bInterrupted);
}

void UComboAttackComponent::ResetCombo()
{
	CurrentMontage = nullptr;
	ComboIndex = 0;
	bIsAttacking = false;
	bNextAttackQueued = false;
	bComboWindowOpen = false;
}

UAnimInstance* UComboAttackComponent::GetOwnerAnimInstance() const
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());

	return (Character && Character->GetMesh()) ? Character->GetMesh()->GetAnimInstance() : nullptr;
}


