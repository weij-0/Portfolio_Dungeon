// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EnemyCharacter.generated.h"

class UHealthComponent;
class AEnemyAIController;
class UBehaviorTree;

UCLASS()
class MYPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/* Properties */

	// Each subclass will set its own specific BT asset in its constructor or Details panel
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset = nullptr;


	/* Functions */

	// Called by the AnimNotify at the impact frame
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteMeleeHit(FName SocketName, float Damage, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteMeleeTrace(FName BaseSocketName, FName TipSocketName, float Damage, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ClearHitIgnoreList();

	/* Setter & Getter */
	FORCEINLINE AEnemyAIController* GetEnemyAIController() const { return CachedEnemyAIController; }

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/* Event Handlers */
	// HealthComponent broadcast
	UFUNCTION()
	void HandleOnHealthChanged(float Percent, float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void HandleOnDeath(AController* InstigatedBy);

	/* Health Components & Properties */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;

private:

	/* Cache */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemyAIController> CachedEnemyAIController = nullptr;

	// Trace Hit Ignore List
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> HitIgnoreList;

	FVector LastBaseLocation;
	FVector LastTipLocation;
	bool bHasLastLocation = false;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float EnemyMaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float EnemyCurrentHealth = 100.0f;
};
