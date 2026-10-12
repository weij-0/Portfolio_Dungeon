// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnManaChanged, float, Percent, float, CurrentMana, float, MaxMana);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManaComponent();

	/* Broadcast */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnManaChanged OnManaChanged;

	/* Functions */

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void InitializeMana(float InitMaxMana, float InitCurrentMana);

	/* Getter / Setter */
	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetMaxMana() const { return MaxMana; }

	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetCurrentMana() const { return CurrentMana; }

	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetCurrentManaPercent() const { return (MaxMana > 0.0f) ? CurrentMana / MaxMana : 0.0f; }

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	/* Properties */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	float MaxMana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	float CurrentMana;

	/* Functions */

	// OnSkillCast multi-cast event handler
	UFUNCTION()
	void HandleSkillCasted(float ManaCost);

};
