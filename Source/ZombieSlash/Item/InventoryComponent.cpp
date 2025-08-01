// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryComponent.h"
#include "Item/ItemData.h"
#include "Item/WeaponData.h"
#include "Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

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
	if (WeaponSlots.IsValidIndex(SlotIdx) && WeaponSlots[SlotIdx].WeaponData)
	{
		CurWeaponSlotIdx = SlotIdx;
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - SwitchWeapon - Slot Idx was Invalid"));
	return false;
}

// 인벤토리에서 무기를 장비할 때 호출
// 그냥
// 슬롯에 뭔가 넣을 때마다 스폰/파괴하도록 하는게 낫지 않을까 (input으론 무기 정보만 받아서 생성, 액터와 정보 세팅)
// 아니면
// WeaponData만 변경하고, 인벤토리를 닫을 때 InitilaizeWeaponSlot 호출하는 것은?
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

	//WeaponSlots[SlotNumber] = Cast<UWeaponData>(Item);
	WeaponSlots[SlotNumber].WeaponData = Cast<UWeaponData>(Item);

	// bEquippedWeaponChanged = true;

	return true;
}

const UWeaponData* UInventoryComponent::GetCurWeaponData() const
{
	return WeaponSlots.IsValidIndex(CurWeaponSlotIdx) ? WeaponSlots[CurWeaponSlotIdx].WeaponData : nullptr;
}

// 무기 슬롯의 클래스 정보 바탕으로 액터를 인스턴스를 생성해 채운다
// 편집기에서 무기 슬롯 중 WeaponData는 편집 가능해야 함
void UInventoryComponent::InitializeWeaponSlots()
{
	for (int32 i = 0; i < WeaponSlotCount; ++i)
	{
		AWeaponBase* WeaponInst = GetWorld()->SpawnActor<AWeaponBase>(WeaponSlots[i].WeaponData->WeaponActorClass);
		WeaponSlots[i].WeaponActor = WeaponInst;
		//WeaponInst->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponInst->SocketName);
		WeaponInst->InitializeWeapon(WeaponSlots[i].WeaponData, GetOwner());
		WeaponInst->OnUnequip();
	}
}

bool UInventoryComponent::AddItem(UItemData* InItemData, int32 InQuantity)
{
	if (!IsValid(InItemData) || InQuantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem Called, but Invalid Input"));
		return false;
	}

	if (!(InItemData->GetPrimaryAssetId().IsValid()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemID in InItemData"));
		return false;
	}

	if (InItemData->ItemType == EItemType::HealItem)
	{
		CurHealItemID = InItemData->GetPrimaryAssetId();
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

int32 UInventoryComponent::GetItemCountByID(FPrimaryAssetId ID) const
{
	int32 Cnt = 0;
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i].ItemData) && Items[i].ItemData->GetPrimaryAssetId() == ID)
		{
			return Items[i].Quantity;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("아이템 없음"));
	return 0;
}

int32 UInventoryComponent::RemoveItem(FPrimaryAssetId ItemID)
{
	int32 ItemIdx = FindItemSlotIndexByID(ItemID);

	if (ItemIdx == INDEX_NONE || !Items.IsValidIndex(ItemIdx)) return false;

	Items[ItemIdx].ItemData = nullptr;
	Items[ItemIdx].Quantity = 0;

	return int32();
}

UItemData* UInventoryComponent::GetItem(FPrimaryAssetId ItemID)
{
	if (FindItemSlotIndexByID(ItemID) == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Index"));
	}
	if (!Items[FindItemSlotIndexByID(ItemID)].IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Item"));
	}

	return Items[FindItemSlotIndexByID(ItemID)].ItemData;
}

bool UInventoryComponent::UseItem(FPrimaryAssetId ItemID, int32 UseQuantity)
{
	int32 ItemIdx = FindItemSlotIndexByID(ItemID);

	if (ItemIdx == INDEX_NONE || !Items.IsValidIndex(ItemIdx))
	{
		UE_LOG(LogTemp, Warning, TEXT("Use Item Fail, Item Index was %d"), ItemIdx);
		return false;
	}
	Items[ItemIdx].Quantity -= UseQuantity;

	if (Items[ItemIdx].Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 0이 되어 삭제"));
		RemoveItem(ItemID);
	}

	return true;
}

void UInventoryComponent::PlayPickupSound()
{
	if (!PickupSound)
		return;

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		PickupSound,
		GetOwner()->GetActorLocation(),
		1.0f,
		1.0f,
		0.0f,
		nullptr,
		nullptr
	);
}

int32 UInventoryComponent::FindStackableSlotIndex(const UItemData* ItemData) const
{
	return int32();
}

int32 UInventoryComponent::FindEmptySlotIndex() const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemData == nullptr || !Items[i].ItemData->GetPrimaryAssetId().IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Empty Slot Index: %d"), i);
			return i;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Empty Slot Index: -1"));
	return INDEX_NONE;
}

int32 UInventoryComponent::FindItemSlotIndexByID(FPrimaryAssetId InID) const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i].ItemData) && Items[i].ItemData->GetPrimaryAssetId() == InID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}