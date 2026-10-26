// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

UItemBase::UItemBase()
{
	Quantity = 1;
	ItemID = NAME_None;
	ItemType = EItemType::Herb;
	ItemRarity = EItemRarity::Common;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(GetOuter(), GetClass());

	if (ItemCopy)
	{
		ItemCopy->Quantity = Quantity;
		ItemCopy->ItemID = ItemID;
		ItemCopy->ItemType = ItemType;
		ItemCopy->ItemRarity = ItemRarity;
		ItemCopy->TextData = TextData;
		ItemCopy->NumericData = NumericData;
		ItemCopy->AssetData = AssetData;
	}
	
	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
	}
}

void UItemBase::UseItem(APlayerCharacter* Character)
{
}
