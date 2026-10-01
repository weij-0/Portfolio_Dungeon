// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EnemyCharacter.generated.h"

class AEnemyAIController;
class UBehaviorTree;

UCLASS()
class MYPROJECT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/* Properties */

	// Each subclass will set its own specific BT asset in its constructor or Details panel
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset = nullptr;


	/* Functions */

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
	void HandleOnDeath(AActor* DamagedActor, AController* InstigatedBy);

	UFUNCTION()
	virtual float TakeDamage( float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:

	/* Cache */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemyAIController> CachedEnemyAIController = nullptr;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float EnemyMaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float EnemyCurrentHealth = 100.0f;

	// Boss Phases
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health|Boss", meta = (AllowPrivateAccess = "true"))
	bool bHasPhaseTwo = false;

};
