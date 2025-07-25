// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryComponent.h"
#include "Item/ItemData.h"
#include "Item/WeaponData.h"

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

// 플레이어가 무기를 교체했을 때 호출
bool UInventoryComponent::SwitchWeapon(int32 SlotIdx)
{
	if (WeaponSlots.IsValidIndex(SlotIdx) && WeaponSlots[SlotIdx])
	{
		CurWeaponSlotIdx = SlotIdx;
		OnWeaponChanged.Broadcast(GetCurWeapon());
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - SwitchWeapon - Slot Idx was Invalid"));
	return false;
}

// 인벤토리에서 무기를 장비할 때 호출
bool UInventoryComponent::EquipWeapon(int32 InventoryIndex, int32 SlotNumber)
{
	if (!Items.IsValidIndex(InventoryIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - EquipWeapon - InventoryIndex was Invalid"));
		return false;
	}
	UItemData* Item = Items[InventoryIndex].ItemData;

	if (Item->ItemType != EItemType::Weapon)
	{

		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - EquipWeapon - NOT Weapon"));
		return false;
	}

	WeaponSlots[SlotNumber] = Cast<UWeaponData>(Item);

	// bEquippedWeaponChanged = true;

	return true;
}

const UWeaponData* UInventoryComponent::GetCurWeapon() const
{
	return WeaponSlots.IsValidIndex(CurWeaponSlotIdx) ? WeaponSlots[CurWeaponSlotIdx] : nullptr;
}

bool UInventoryComponent::AddItem(UItemData* InItemData, int32 InQuantity)
{
	if (!IsValid(InItemData) || InQuantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem Called, but Invalid Input"));
		return false;
	}

	if (!InItemData->ItemID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemID in InItemData"));
		return false;
	}

	if (InItemData->ItemType == EItemType::HealItem)
	{
		CurHealItemID = InItemData->ItemID;
	}


	int32 Remaining = InQuantity;

	// 스택 가능한 슬롯 찾기
	for (FInventorySlot& ItemSlot : Items)
	{
		const UItemData* Item = ItemSlot.ItemData;
		if (!IsValid(Item)) continue;
		if (Item->ItemID == InItemData->ItemID)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Stackable Slot"));

			int32 Space = InItemData->MaxStack - ItemSlot.Quantity;
			int32 ToAdd = FMath::Min(Space, Remaining);
			ItemSlot.Quantity += ToAdd;
			Remaining -= ToAdd;

			if (Remaining <= 0)
				return true;
		}
	}

	// 빈 슬롯에 넣기
	while (Remaining > 0)
	{
		int32 SlotIndex = FindEmptySlotIndex();

		if (SlotIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Full!"));
			return false;
		}

		int32 ToAdd = FMath::Min(InItemData->MaxStack, Remaining);
		Items[SlotIndex] = FInventorySlot{ InItemData, ToAdd };
		Remaining -= ToAdd;
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
		UE_LOG(LogTemp, Warning, TEXT("Use Item Fail, Item Index was %d"), ItemIdx);
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