// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryComponent.h"
#include "Item/ItemData.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}


bool UInventoryComponent::AddItem(const UItemData* InItemData, int32 InQuantity)
{
	if (nullptr == InItemData || InQuantity <= 0) return false;

	// ½ºÅÃ °¡´ÉÇÑ ½½·Ô Ã£±â
	for (FInventorySlot& ItemSlot : Items)
	{
		const UItemData* Item = ItemSlot.ItemData;
		if (Item->ItemID == InItemData->ItemID)
		{
			ItemSlot.Quantity += InQuantity;
			return true;
		}
	}

	// ºó ½½·Ô¿¡ ³Ö±â
	int32 SlotIndex = FindEmptySlotIndex();
	if (SlotIndex == INDEX_NONE)
	{
		Items[SlotIndex] = FInventorySlot(InItemData, InQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Full!"));
		return false;
	}
	
	return true;
}

int32 UInventoryComponent::GetItemCount(FName ItemID) const
{
	return int32();
}

int32 UInventoryComponent::RemoveItem(FName ItemID) const
{
	return int32();
}

int32 UInventoryComponent::FindStackableSlotIndex(const UItemData* ItemData) const
{
	return int32();
}

int32 UInventoryComponent::FindEmptySlotIndex() const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemData->ItemID.IsNone())
		{
			return i;
		}
	}

	return INDEX_NONE;
}
