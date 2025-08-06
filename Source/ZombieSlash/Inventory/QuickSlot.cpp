// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/QuickSlot.h"
#include "Item/ItemManagerSubsystem.h"
#include "Gamedata/ItemMetaData.h"

UQuickSlot::UQuickSlot()
{
	MaxSlotCount = 2;
	Items.SetNum(MaxSlotCount);
}

void UQuickSlot::TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory)
{
	FInventorySlot SrcSlot = SrcInventory->Items[SrcIdx];

	UItemManagerSubsystem* ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();

	FItemMetadata Metadata;
	if (!ItemManager->GetMetadata(SrcSlot.ItemID, Metadata)) return;
	if (Metadata.ItemType != EItemType::Usable) return;

	Super::TransferSlot(DestIdx, SrcIdx, SrcInventory);
}
