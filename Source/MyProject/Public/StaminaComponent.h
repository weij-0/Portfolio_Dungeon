// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaChanged, float, Percent, float, CurrentStamina, float, MaxStamina);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	/* Broadcast */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaChanged OnStaminaChanged;

	/* Functions */

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void InitializeStamina(float InitMaxStamina, float InitCurrentStamina);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateStamina(float NewStamina);

	/* Getter / Setter */
	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetCurrentStaminaPercent() const { return (MaxStamina > 0.0f) ? CurrentStamina / MaxStamina : 0.0f; }

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	/* Properties */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;


		
};
