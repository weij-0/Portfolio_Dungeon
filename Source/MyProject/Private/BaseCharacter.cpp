// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Projectile.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	WeaponMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshRight"));
	WeaponMeshRight->SetupAttachment(GetMesh(), FName("WeaponSocket_r"));

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh(), FName("MuzzleSocket"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnHitReact.AddDynamic(this, &ABaseCharacter::HandleOnHitReact);
	}
}

void ABaseCharacter::ExecuteMeleeHit(FName SocketName, float Damage, float Radius)
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance() || SocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecuteMeleeHit: Invalid parameters or missing components."));
		return;
	}

	// Set up the collision shape for the trace
	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);

	FCollisionShape HitShape = FCollisionShape::MakeSphere(Radius);

	//DrawDebugSphere(GetWorld(), SocketLocation, HitShape.GetSphereRadius(), 12, FColor::Red, false, 1.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self

	TArray<FHitResult> HitResults;

	// Perform Multi-Sphere Trace for Pawn channel
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SocketLocation,
		SocketLocation + (GetActorForwardVector() * 1.0f),
		FQuat::Identity,
		ECC_Pawn, // Detect Player Pawn
		HitShape,
		QueryParams
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this)
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					Damage,
					GetController(),
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

void ABaseCharacter::ExecuteMeleeTrace(FName BaseSocketName, FName TipSocketName, float Damage, float TraceRadius)
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance() || BaseSocketName.IsNone() || TipSocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecuteMeleeTrace: Invalid parameters or missing components."));
		return;
	}

	// Dynamically sample WeaponMeshRight first, fallback to Character Skeletal Mesh
	USceneComponent* TargetWeaponMesh = (WeaponMeshRight)
		? Cast<USceneComponent>(WeaponMeshRight)
		: Cast<USceneComponent>(GetMesh());

	if (!TargetWeaponMesh->DoesSocketExist(BaseSocketName) || !TargetWeaponMesh->DoesSocketExist(TipSocketName))
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] MeleeTrace: Sockets '%s' or '%s' NOT FOUND on %s!"), *GetName(), *BaseSocketName.ToString(), *TipSocketName.ToString(), *TargetWeaponMesh->GetName());
		return;
	}

	FVector CurrentBaseLocation = TargetWeaponMesh->GetSocketLocation(BaseSocketName);
	FVector CurrentTipLocation = TargetWeaponMesh->GetSocketLocation(TipSocketName);

	// Initialize the last locations and exit
	if (!bHasLastLocation)
	{
		LastBaseLocation = CurrentBaseLocation;
		LastTipLocation = CurrentTipLocation;
		bHasLastLocation = true;
		return;
	}

	// Set up the collision shape for the trace

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self
	QueryParams.AddIgnoredActors(HitIgnoreList); // Ignore previously hit actors

	// Multi-point swept sphere check across current and previous frame socket locations
	const int32 TraceSubdivisions = 4; // Number of subdivisions for the trace

	FCollisionShape HitShape = FCollisionShape::MakeSphere(TraceRadius);

	for (int32 i = 0; i <= TraceSubdivisions; ++i)
	{
		float Alpha = (float)i / (float)TraceSubdivisions;
		FVector PreviousPointOnBlade = FMath::Lerp(LastBaseLocation, LastTipLocation, Alpha);
		FVector CurrentPointOnBlade = FMath::Lerp(CurrentBaseLocation, CurrentTipLocation, Alpha);

		TArray<FHitResult> HitResults;

		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResults,
			PreviousPointOnBlade,
			CurrentPointOnBlade,
			FQuat::Identity,
			ECC_Pawn,
			HitShape,
			QueryParams
		);

#if WITH_EDITOR
		// Visualize the blade arc motion in editor
		//DrawDebugLine(GetWorld(), PreviousPointOnBlade, CurrentPointOnBlade, FColor::Red, false, 1.0f);
		//DrawDebugSphere(GetWorld(), CurrentPointOnBlade, TraceRadius, 8, FColor::Green, false, 1.0f);
#endif

		if (bHit)
		{
			for (const FHitResult& Hit : HitResults)
			{
				AActor* HitActor = Hit.GetActor();
				if (HitActor && HitActor != this && !HitIgnoreList.Contains(HitActor))
				{
					UGameplayStatics::ApplyDamage(
						HitActor,
						Damage,
						GetController(),
						this,
						UDamageType::StaticClass()
					);
					HitIgnoreList.Add(HitActor);
				}
			}
		}

	LastBaseLocation = CurrentBaseLocation;
	LastTipLocation = CurrentTipLocation;

	}
}

void ABaseCharacter::ClearHitIgnoreList()
{
	HitIgnoreList.Empty();
	HitIgnoreList.Add(this); // Ignore self
	bHasLastLocation = false;
}

void ABaseCharacter::SpawnProjectile()
{
	// Owner actor location & rotation
	FVector ForwardVector = GetActorForwardVector();
	FRotator SpawnRotation = GetActorRotation();

	// Distance offsets
	FVector BaseLocation = ProjectileSpawnPoint ? ProjectileSpawnPoint->GetComponentLocation() : GetActorLocation();
	FVector FinalSpawnLocation = BaseLocation + (ForwardVector * ProjectileForwardOffset) + (0.0f, 0.0f, ProjectileHeightOffset);

	// Spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn projectile
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FinalSpawnLocation, SpawnRotation, SpawnParams);

}

void ABaseCharacter::HandleOnHitReact(AActor* DamageCauser)
{
	if (!DamageCauser || !GetMesh() || !GetMesh()->GetAnimInstance()) return;

	// Calculate 3D Directional Angle to select appropriate Hit React Montage
	FVector HitDirection = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector LocalHitDirection = GetActorTransform().InverseTransformVectorNoScale(HitDirection);

	float ForwardDot = LocalHitDirection.X;
	float RightDot = LocalHitDirection.Y;

	UAnimMontage* MontageToPlay = HitReactFront;

	if (ForwardDot < -0.5f)
	{
		MontageToPlay = HitReactBack ? HitReactBack : HitReactFront;
	}
	else if (FMath::Abs(RightDot) > FMath::Abs(ForwardDot))
	{
		if (RightDot > 0.0f)
		{
			MontageToPlay = HitReactRight ? HitReactRight : HitReactFront;
		}
		else
		{
			MontageToPlay = HitReactLeft ? HitReactLeft : HitReactFront;
		}
	}

	if (MontageToPlay)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
	}
}

