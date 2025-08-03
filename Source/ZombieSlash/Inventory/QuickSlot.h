// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "QuickSlot.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBIESLASH_API UQuickSlot : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UQuickSlot();

protected:
	virtual void TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory) override;
};
