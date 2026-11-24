// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

class AMyPlayerController;
class UWidgetComponent;
class UInventoryComponent;
class UInteractionInterface;
class UInteractTraceComponent;
class UInteractableComponent;
class UStaticMeshComponent;
class UComboAttackComponent;
class UAnimMontage;

UCLASS()
class MYPROJECT_API APlayerCharacter : public ABaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Team 0 = Player
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(0); }

	// Setters & Getters
	void SetGameplaySuspended(bool bSuspended);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* PlayerController) override;

	/* Input Function Calls */

	// Move
	void Move(const FInputActionValue& Value);

	// Look
	void Look(const FInputActionValue& Value);

	// Sprint
	void SprintStarted(const FInputActionValue& Value);
	void SprintCompleted(const FInputActionValue& Value);
	void StopSprint();

	// Crouch
	void CrouchStarted(const FInputActionValue& Value);
	void CrouchCompleted(const FInputActionValue& Value);

	// Jump & Dash
	void JumpOrDash(const FInputActionValue& Value);

	// Interact (E)
	void Interact(const FInputActionValue& Value);

	// Inventory Input (I)
	void ToggleInventoryPressed(const FInputActionValue& Value);

	// Attack (LMB)
	void Attack(const FInputActionValue& Value);

	// Stamina Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* StaminaWidgetComponent;

	// Inventory Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* PlayerInventoryComponent;

	// Combo Attack Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ComboAttack")
	UComboAttackComponent* ComboAttackComponent;

	// Active UI widget instance cache
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget* InventoryWidgetScreen;

	// Blueprint event fires when input button clicked
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnToggleInventoryEvent();

	/* Event Handlers */
	// StaminaComponent broadcast
	UFUNCTION()
	void HandleOnDeath(AActor* DamagedActor, AController* InstigatedBy);
	
	UFUNCTION()
	void HandleOnStaminaChanged(float NewPercent, float NewCurrentStamina, float NewMaxStamina);


private:
	/* Cache */
	UPROPERTY(Transient)
	AMyPlayerController* CachedPlayerController;

	/* Camera Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;


	/* Enhanced Input Assets */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ToggleInventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	/* Initialize Settings */

	UPROPERTY(EditAnywhere, Category = "Speed")
	float NormalWalkSpeed = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float NormalWalkSpeedCrouch = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float SprintSpeed = 700.0f;
	
	// Dash
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashForce = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDuration = 0.2f;

	bool bCanDash = false;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float PlayerMaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float PlayerCurrentHealth;

	// Mana
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana", meta = (AllowPrivateAccess = "true"))
	float MaxMana = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana", meta = (AllowPrivateAccess = "true"))
	float CurrentMana;

	// Stamina
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float MaxStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float SprintStaminaCost = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float DashStaminaCost = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenDelay = 2.0f;

	bool bIsSprint = false;
	bool bIsDash = false;

	FTimerHandle StaminaRegenDelayTimerHandle;

	void StaminaDeplete(float DeltaTime, float StaminaCost);
	void StaminaRegen(float DeltaTime);


	/* Stamina Widget */

	UPROPERTY(EditAnywhere, Category = "UI")
	float StaminaWidgetFadeDelay = 2.0f;

	FTimerHandle StaminaWidgetFadeTimerHandle;

	void HideStaminaWidget();

	/* Interaction */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractTraceComponent> InteractTraceComponent;


	
};
