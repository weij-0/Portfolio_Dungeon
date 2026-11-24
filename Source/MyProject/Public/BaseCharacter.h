// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UManaComponent;
class UStaminaComponent;
class UStaticMeshComponent;
class UAnimMontage;
class AProjectile;
class USceneComponent;
class UInputAction;
class UWidgetComponent;

UCLASS()
class MYPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	/* Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAlive = true;

	/* Functions */

	// Called by the AnimNotify at the impact frame
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteMeleeHit(FName SocketName, float Damage, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteMeleeTrace(FName BaseSocketName, FName TipSocketName, float Damage, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ClearHitIgnoreList();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SpawnProjectile();

	/* Setters & Getters */
	UStaminaComponent* GetStaminaComponent();
	UManaComponent* GetManaComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Event Handlers */
	UFUNCTION()
	virtual void HandleOnHitReact(AActor* DamageCauser);

	/* Health Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;

	/* Mana Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UManaComponent> ManaComponent = nullptr;

	/* Stamina Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaminaComponent> StaminaComponent = nullptr;

	/* Weapon Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WeaponMeshRight = nullptr;

	/* Projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> ProjectileSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Projectile")
	float ProjectileForwardOffset = 100.0f; // Distance in front of AI

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Projectile")
	float ProjectileHeightOffset = 0.0f; // Height off the ground (Z-axis, depends on spawn location first)

	/* Melee Trace Cache */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<AActor*> HitIgnoreList;

	FVector LastBaseLocation;
	FVector LastTipLocation;
	bool bHasLastLocation = false;

	/* Directional Hit Reaction Montages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit React")
	TObjectPtr<UAnimMontage> HitReactFront = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit React")
	TObjectPtr<UAnimMontage> HitReactRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit React")
	TObjectPtr<UAnimMontage> HitReactLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit React")
	TObjectPtr<UAnimMontage> HitReactBack = nullptr;

private:	
	

};
