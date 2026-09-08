// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */

/* Declare ENUM */

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
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
	FText Name = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText InteractionText = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText UsageText = FText::GetEmpty();
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStackable = false;
};

USTRUCT(BlueprintType)
struct FItemDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh = nullptr;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType ItemType = EItemType::Consumable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemRarity ItemRarity = EItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemDataAsset AssetData;
};
