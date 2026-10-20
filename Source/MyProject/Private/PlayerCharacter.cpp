// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "MyPlayerController.h"
#include "HealthComponent.h"
#include "ManaComponent.h"
#include "StaminaComponent.h"
#include "StaminaWidget.h"
#include "ItemPickup.h"
#include "InventoryComponent.h"
#include "InteractionInterface.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	/* Camera Boom */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 40.0f, 70.0f));
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 20.0f, 20.0f);
	CameraBoom->bUsePawnControlRotation = true;

	/* Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	/* Stamina Widget Component */
	StaminaWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StaminaWidgetComponent"));
	StaminaWidgetComponent->SetupAttachment(GetMesh(), FName("pelvis"));
	StaminaWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StaminaWidgetComponent->SetDrawAtDesiredSize(true);
	StaminaWidgetComponent->SetVisibility(false);
	StaminaWidgetComponent->SetOnlyOwnerSee(true); // Set only visible to local player

	/* Crouch */
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	/* MaxWalkSpeed */
	GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;

	/* Health */
	PlayerCurrentHealth = PlayerMaxHealth;

	/* Mana */
	CurrentMana = MaxMana;
	
	/* Stamina */
	CurrentStamina = MaxStamina;

	/* Inventory Component */
	PlayerInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventoryComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Cache Controller
void APlayerCharacter::PossessedBy(AController* PlayerController)
{
	Super::PossessedBy(PlayerController);

	CachedPlayerController = Cast<AMyPlayerController>(PlayerController);

	// Initialize Health Component
	if (HealthComponent)
	{
		PlayerCurrentHealth = PlayerMaxHealth;
		HealthComponent->InitializeHealth(PlayerMaxHealth, PlayerCurrentHealth);

		HealthComponent->OnDeath.AddUniqueDynamic(this, &APlayerCharacter::HandleOnDeath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter: HealthComponent is not valid!"));
	}

	// Initialize Mana Component
	if (GetManaComponent())
	{
		CurrentMana = MaxMana;
		ManaComponent->InitializeMana(MaxMana, CurrentMana);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter: ManaComponent is not valid!"));
	}

	// Initialize Stamina Component
	if (GetStaminaComponent())
	{
		StaminaComponent->OnStaminaChanged.AddUniqueDynamic(this, &APlayerCharacter::HandleOnStaminaChanged);

		CurrentStamina = MaxStamina;
		StaminaComponent->InitializeStamina(MaxStamina, CurrentStamina);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter: StaminaComponent is not valid!"));
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform Line Trace every frame to detect Interactable Actors
	PerformInteractionTrace();

	if (GetStaminaComponent())
	{
		if (bIsSprint && CurrentStamina > 0.0f)
		{
			StaminaDeplete(DeltaTime, SprintStaminaCost);
		}
		else
		{
			if (bIsSprint)
			{
				StopSprint(); // Also trigger (bIsSprint = false) inside.
			}

			if (!GetWorldTimerManager().IsTimerActive(StaminaRegenDelayTimerHandle) && CurrentStamina < MaxStamina)
			{
				StaminaRegen(DeltaTime);
			}
		
		}
	}
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move & Look
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Jump (Started -> once per input, Hold not allowed)
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpOrDash);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::SprintStarted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintCompleted);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::CrouchStarted);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::CrouchCompleted);

		// Interact (F)
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::InteractButtonItemPickup);

		// Inventory (I)
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventoryPressed);

		// Attack (LMB)
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);

	}
}

// Move Action
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// Look Action
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Sprint Action
void APlayerCharacter::SprintStarted(const FInputActionValue& Value)
{
	if (CurrentStamina > 0.0f)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetWorldTimerManager().ClearTimer(StaminaRegenDelayTimerHandle);
	}
}

void APlayerCharacter::SprintCompleted(const FInputActionValue& Value)
{
	StopSprint();
}

void APlayerCharacter::StopSprint()
{
	if (bIsSprint) GetWorldTimerManager().SetTimer(StaminaRegenDelayTimerHandle, StaminaRegenDelay, false);

	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
}

// Crouch Action
void APlayerCharacter::CrouchStarted(const FInputActionValue& Value)
{
	StopSprint();
	Crouch();
}

void APlayerCharacter::CrouchCompleted(const FInputActionValue& Value)
{
	UnCrouch();
}

// Jump or Dash Action
void APlayerCharacter::JumpOrDash(const FInputActionValue& Value)
{
	if (!GetCharacterMovement()->IsFalling()) // If on ground
	{
		if (bIsSprint) StopSprint();

		Jump();
		bCanDash = true;

	}
	else if (GetCharacterMovement()->IsFalling() && bCanDash && CurrentStamina >= DashStaminaCost) // If Airborned, Can Dash, Enough Stamina
	{
		FVector DashDirection = GetCharacterMovement()->GetLastInputVector();

		if (DashDirection.IsNearlyZero())
		{
			DashDirection = GetActorForwardVector();
		}
		else
		{
			DashDirection.Normalize();
		}

		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		LaunchCharacter(DashDirection * DashForce, true, true);
		bCanDash = false;
		
		if (GetStaminaComponent())
		{
			// Deplete Stamina
			CurrentStamina = FMath::Clamp(CurrentStamina - DashStaminaCost, 0.0f, MaxStamina);
			StaminaComponent->UpdateStamina(CurrentStamina);

			// Ready for Regen
			GetWorldTimerManager().SetTimer(StaminaRegenDelayTimerHandle, StaminaRegenDelay, false);
		}
	}
}

// Interact Action (F)
void APlayerCharacter::InteractButtonItemPickup(const FInputActionValue& Value)
{
	AActor* Target = CurrentTargetInteraction.Get();

	if (Target && Target->Implements<UInteractionInterface>())
	{
		// Trigger the interaction logic in the interacting actor
		IInteractionInterface::Execute_Interact(Target, this);
	}
}

// Toggle Inventory Action (I)
void APlayerCharacter::ToggleInventoryPressed(const FInputActionValue& Value)
{
	// Fires the Event up to Blueprints
	OnToggleInventoryEvent();
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (!BasicAttack)
	{
		UE_LOG(LogTemp, Error, TEXT("Attack Failed: 'BasicAttack' Montage property is NULL! Assign it in BP details."));
		return;
	}

	if (!GetMesh() || !GetMesh()->GetAnimInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("Attack Failed: Mesh or AnimInstance is missing on character!"));
		return;
	}

	float Duration = PlayAnimMontage(BasicAttack);
	UE_LOG(LogTemp, Warning, TEXT("PlayAnimMontage Executed. Returned Duration: %f seconds"), Duration);

}

// Stamina

void APlayerCharacter::StaminaDeplete(float DeltaTime, float StaminaCost)
{
	// Calculate new stamina value
	if (CurrentStamina <= 0.0f)
	{
		CurrentStamina = 0.0f;
		StopSprint();
	}
	else
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - (StaminaCost * DeltaTime), 0.0f, MaxStamina);
	}

	// Update StaminaComponent with the new value
	StaminaComponent->UpdateStamina(CurrentStamina);
}

void APlayerCharacter::StaminaRegen(float DeltaTime)
{
	//Calculate new stamina value
	CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.0f, MaxStamina);
	
	// Update StaminaComponent with the new value
	StaminaComponent->UpdateStamina(CurrentStamina);
}

/* Stamina Widget*/

void APlayerCharacter::HandleOnDeath(AActor* DamagedActor, AController* InstigatedBy)
{
	if (!bIsAlive) return;

	bIsAlive = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
}

void APlayerCharacter::HandleOnStaminaChanged(float NewPercent, float NewCurrentStamina, float NewMaxStamina)
{
	if (!StaminaWidgetComponent) return;

	// If Stamina is not full, show the widget and reset the fade timer
	if (NewPercent < 1.0f)
	{
		GetWorldTimerManager().ClearTimer(StaminaWidgetFadeTimerHandle);

		if (!StaminaWidgetComponent->IsVisible())
		{
			StaminaWidgetComponent->SetVisibility(true);
		}
	}
	else if (NewPercent >= 1.0f && StaminaWidgetComponent->IsVisible() && !GetWorldTimerManager().IsTimerActive(StaminaWidgetFadeTimerHandle))
	{	
		// If Stamina is full, start the fade timer to hide the widget after a delay
		GetWorldTimerManager().SetTimer(StaminaWidgetFadeTimerHandle, this, &APlayerCharacter::HideStaminaWidget, StaminaWidgetFadeDelay, false);
	}
	
}

void APlayerCharacter::HideStaminaWidget()
{
	if (StaminaWidgetComponent)
	{
		StaminaWidgetComponent->SetVisibility(false);
	}
}

void APlayerCharacter::PerformInteractionTrace()
{
	if (!IsLocallyControlled() || !FollowCamera) return;

	/* Old Target*/
	AActor* OldTarget = CurrentTargetInteraction.Get();
	AActor* NewTarget = nullptr;

	/* Line tracing from camera to endpoints */
	const FVector StartPoint = FollowCamera->GetComponentLocation();
	const FVector ForwardVector = FollowCamera->GetForwardVector();
	const FVector EndPoint = StartPoint + (ForwardVector * InteractionTraceDistance);

	// Physics query parameters
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, TraceParams))
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->Implements<UInteractionInterface>())
		{
			NewTarget = HitActor;
		}
	}

	// If target changed (Target hit / lost)
	if (NewTarget != OldTarget)
	{
		// Hide UI prompt on old target
		if (OldTarget)
		{
			IInteractionInterface::Execute_HideInteractionPrompt(OldTarget);
		}

		CurrentTargetInteraction = NewTarget;

		// Show UI prompt on new target
		if (NewTarget)
		{
			IInteractionInterface::Execute_ShowInteractionPrompt(NewTarget);
		}
		
		if (CachedPlayerController && CachedPlayerController->HUDWidget)
		{
			CachedPlayerController->HUDWidget->SetCrosshairVisibility(NewTarget != nullptr);
		}
	}
	// If LineTrace didn't hit anything
	else if (!NewTarget && !CurrentTargetInteraction.IsExplicitlyNull())
	{
		// Clear the current target if we previously had one (Also clear if previous target is destroyed)
		CurrentTargetInteraction = nullptr;

		// Hide UI prompt on old target
		if (CachedPlayerController && CachedPlayerController->HUDWidget)
		{
			CachedPlayerController->HUDWidget->SetCrosshairVisibility(false);
		}
	}
	
}
