// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Components/StaticMeshComponent.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */

/* Declare ENUM */

UENUM()
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UENUM()
enum class EItemType : uint8
{
	Herb UMETA(DisplayName = "Herb"),
	Quest UMETA(DisplayName = "Quest Item"),
	Consumable UMETA(DisplayName = "Consumable Item")
};


/* Declare Structs */
USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText InteractionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText UsageText;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FItemDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemDataAsset AssetData;
};
