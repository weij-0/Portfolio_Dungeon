// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboAttackComponent.generated.h"

class UAnimMontage;
class UAnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboStepChanged, int32, ComboIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboEnded, bool, bInterrupted);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UComboAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComboAttackComponent();

	/* Functions */
	
	// Combo Window (For AnimNotifyState)
	UFUNCTION()
	void OpenComboWindow();

	UFUNCTION()
	void CloseComboWindow();

	// Attack/Reset (from PlayerCharacter)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool RequestAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetCombo();

	/* Delegates */

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnComboStepChanged OnComboStepChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnComboEnded OnComboEnded;

protected:
	/* Properties */

	// Animation Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Attack")
	TObjectPtr<UAnimMontage> CurrentMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Attack")
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;

private:	

	/* Functions */

	bool PlayMontage(int32 Index);

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UAnimInstance* GetOwnerAnimInstance() const;

	/* Properties */

	// Flags
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	bool bNextAttackQueued = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	bool bComboWindowOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	int32 ComboIndex = 0;

};
