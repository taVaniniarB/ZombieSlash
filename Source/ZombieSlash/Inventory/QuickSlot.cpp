// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/QuickSlot.h"

UQuickSlot::UQuickSlot()
{
	MaxSlotCount = 2;
	Items.SetNum(MaxSlotCount);
}

void UQuickSlot::TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory)
{
	FInventorySlot SrcSlot = SrcInventory->Items[SrcIdx];
	const UItemData* ItemData = GetItemData(SrcSlot.ItemID);

	if (!ItemData || !(ItemData->ItemType == EItemType::Usable))
	{
		// ���� Ÿ���� �ƴϸ� �ƹ� ���۵� ���� ����
		return;
	}

	Super::TransferSlot(DestIdx, SrcIdx, SrcInventory);
}
