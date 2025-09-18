// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/WeaponSlot.h"
#include "Item/ItemManagerSubsystem.h"
#include "Gamedata/ItemMetaData.h"
#include "Item/WeaponData.h"

UWeaponSlot::UWeaponSlot()
{

}


void UWeaponSlot::BeginPlay()
{
	Super::BeginPlay();
}

const UWeaponData* UWeaponSlot::GetCurWeaponData(int32 InIndex) const
{
	return WeaponData.IsValidIndex(InIndex) ? WeaponData[InIndex] : nullptr;
}

void UWeaponSlot::SetInventorySize(int32 InMaxSlotCount)
{
	Super::SetInventorySize(InMaxSlotCount);

	WeaponData.SetNumUninitialized(InMaxSlotCount);
}

void UWeaponSlot::TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory)
{
	FInventorySlot SrcSlot = SrcInventory->Items[SrcIdx];
	FPrimaryAssetId SrcID = SrcSlot.ItemID;

	UItemManagerSubsystem* ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();

	FItemMetadata Metadata;
	if (!ItemManager->GetMetadata(SrcID, Metadata)) return;
	if (Metadata.ItemType != EItemType::Weapon) return;

	Super::TransferSlot(DestIdx, SrcIdx, SrcInventory);
}

void UWeaponSlot::LoadWeaponDataFromItems()
{
	UItemManagerSubsystem* ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();


	// 멤버 변수로 상태 추적
	PendingLoadsCount = Items.Num();
	WeaponData.SetNumUninitialized(Items.Num());

	if (PendingLoadsCount == 0)
	{
		OnUpdatedWeaponSlot.ExecuteIfBound();
		return;
	}

	// Items 내 모든 무기 데이터 로드
	for (int Idx = 0; Idx < Items.Num(); ++Idx)
	{
		FInventorySlot& Item = Items[Idx];
		if (!Item.IsValid())
		{
			WeaponData[Idx] = nullptr;
			--PendingLoadsCount;
			if (PendingLoadsCount == 0)
			{
				OnUpdatedWeaponSlot.ExecuteIfBound();
			}
			continue;
		}

		ItemManager->LoadItemDataAsync(Item.ItemID, [this, Idx](UItemData* ItemData)
			{
				UWeaponData* Weapon = Cast<UWeaponData>(ItemData);
				WeaponData[Idx] = Weapon;

				--PendingLoadsCount;
				if (PendingLoadsCount == 0)
				{
					OnUpdatedWeaponSlot.ExecuteIfBound();
				}
			});
	}
}

void UWeaponSlot::RegistWeaponData(UItemData* ItemData, int32 Idx)
{
	UWeaponData* Weapon = Cast<UWeaponData>(ItemData);
	WeaponData[Idx] = Weapon;
}

void UWeaponSlot::UpdateInventory()
{
	bEquippedWeaponChanged = true;
}
