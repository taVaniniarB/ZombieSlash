// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryComponent.h"
#include "Item/ItemData.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Items.SetNum(MaxSlotCount);
	// EquippedWeapons =
}



//const TArray<TObjectPtr<const class UWeaponData>>& UInventoryComponent::GetEquippedWeapons() const
//{
//	return EquippedWeapons;
//}

bool UInventoryComponent::EquipWeapon(int32 InventoryIndex, int32 SlotNumber)
{
	if (!Items.IsValidIndex(InventoryIndex)) return false;

	const UItemData* Item = Items[InventoryIndex].ItemData;
	if (Item->ItemType != EItemType::Weapon) return false;

	if (SlotNumber == 1)
	{
		EquippedWeaponSlot1 = Items[InventoryIndex];
	}
	else if (SlotNumber == 2)
	{
		EquippedWeaponSlot2 = Items[InventoryIndex];
	}
	else
	{
		return false;
	}

	//EquippedWeapons[SlotNumber] = Cast<UWeaponData>(Item);

	// Character = Cast<IInventoryInterface>(GetOwner());
	// Character->EquipWeaponChanged()
	// 이렇게 하지말고 EquippedWeaponChanged라는 델리게이트를 만드는 게 좋나?
	// Character->OnEquippedWeaponChanged.broadcast();

	return true;
}

bool UInventoryComponent::AddItem(const UItemData* InItemData, int32 InQuantity)
{
	if (!IsValid(InItemData) || InQuantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem Called, but Invalid Input"));
		return false;
	}

	// 스택 가능한 슬롯 찾기
	for (FInventorySlot& ItemSlot : Items)
	{
		const UItemData* Item = ItemSlot.ItemData;
		if (!IsValid(Item)) continue;
		if (Item->ItemID == InItemData->ItemID)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Stackable Slot"));
			ItemSlot.Quantity += InQuantity;
			return true;
		}
	}

	// 빈 슬롯에 넣기
	int32 SlotIndex = FindEmptySlotIndex();
	if (SlotIndex != INDEX_NONE)
	{
		Items[SlotIndex] = FInventorySlot(InItemData, InQuantity);
		UE_LOG(LogTemp, Warning, TEXT("Slot Index: %d"), SlotIndex);
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
	int32 ItemIdx = FindItemSlotIndexByID(ItemID);

	if (ItemIdx == INDEX_NONE || Items.IsValidIndex(ItemIdx)) return INDEX_NONE;

	return Items[ItemIdx].Quantity;
}

int32 UInventoryComponent::GetItemCountByType(EItemType ItemType) const
{
	int32 Cnt = 0;
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i].ItemData) && Items[i].ItemData->ItemType == ItemType)
		{
			++Cnt;
		}
	}
	return Cnt;
}

int32 UInventoryComponent::RemoveItem(FName ItemID)
{
	int32 ItemIdx = FindItemSlotIndexByID(ItemID);

	if (ItemIdx == INDEX_NONE || !Items.IsValidIndex(ItemIdx)) return false;

	Items.RemoveAt(ItemIdx);
	// 인벤토리 재정렬
	return int32();
}

bool UInventoryComponent::UseItem(FName ItemID)
{
	int32 ItemIdx = FindItemSlotIndexByID(ItemID);

	if (ItemIdx == INDEX_NONE || !Items.IsValidIndex(ItemIdx))
	{
		UE_LOG(LogTemp, Warning, TEXT("Use Item Fail"));
		return false;
	}
	Items[ItemIdx].Quantity -= 1;

	if (Items[ItemIdx].Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 0이 되어 삭제"));
		RemoveItem(ItemID);
	}

	return true;
}

int32 UInventoryComponent::FindStackableSlotIndex(const UItemData* ItemData) const
{
	return int32();
}

int32 UInventoryComponent::FindEmptySlotIndex() const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemData == nullptr || Items[i].ItemData->ItemID.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("Empty Slot Index: %d"), i);
			return i;
		}

		/*if (Items[i].ItemData->ItemID.IsNone())
		{
			return i;
		}*/
	}

	UE_LOG(LogTemp, Warning, TEXT("Empty Slot Index: -1"));
	return INDEX_NONE;
}

int32 UInventoryComponent::FindItemSlotIndexByID(FName InID) const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i].ItemData) && Items[i].ItemData->ItemID == InID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}