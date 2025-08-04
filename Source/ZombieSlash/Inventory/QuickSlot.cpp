// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/QuickSlot.h"
#include "Item/ItemManagerSubsystem.h"

UQuickSlot::UQuickSlot()
{
	MaxSlotCount = 2;
	Items.SetNum(MaxSlotCount);
}

void UQuickSlot::TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory)
{
	FInventorySlot SrcSlot = SrcInventory->Items[SrcIdx];

	//if (SrcSlot.ItemType != EItemType::Usable) return;

	/*UItemManagerSubsystem* ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();
	ItemManager->LoadItemDataAsync(SrcSlot.ItemID, [](UItemData* ItemData)
		{
			if (!ItemData || ItemData->ItemType != EItemType::Usable)
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Usable Item"));
				return;
			}
		});*/

	Super::TransferSlot(DestIdx, SrcIdx, SrcInventory);
}
