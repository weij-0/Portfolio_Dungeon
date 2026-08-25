// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerBoxComponent.generated.h"

class ALock;
class UMoverAComponent;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UTriggerBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UTriggerBoxComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Functions */

	void Trigger(bool NewTriggerValue);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	/* Properties */

	UPROPERTY(EditAnywhere, Category = Category = "Keys and Tags");
	TArray<FName> AcceptableActorTags;


private:

	/* Functions */

	bool HasAcceptableTag(AActor* ActorToCheck) const;

	/* Properties */

	UPROPERTY(VisibleAnywhere);
	ALock* Lock = nullptr;

	UPROPERTY(VisibleAnywhere);
	UMoverAComponent* Mover = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool bIsTriggered = false;

	UPROPERTY(VisibleAnywhere);
	int32 ActivatorCount = 0;
};
