// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "ItemBase.generated.h"


class APlayerCharacter;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYPROJECT_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:

	/* Properties & Variables */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (UIMin=1, UIMax=99))
	int32 Quantity;

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

	/* Functions */

	UItemBase();

	UItemBase* CreateItemCopy(UObject* NewOuter) const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return NumericData.Weight * Quantity; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; }
	
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void UseItem(APlayerCharacter* Character);

protected:

	bool operator==(const UItemBase& Other) const
	{
		return ItemID == Other.ItemID;
	}

};
