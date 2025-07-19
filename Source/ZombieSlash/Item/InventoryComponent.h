// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ItemPickup.h"

#include "InventoryComponent.generated.h"


USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot(const UItemData* InItemData, int32 InQuantity)
		: ItemData(InItemData), Quantity(InQuantity)
	{}

	FInventorySlot()
		: ItemData(nullptr), Quantity(0)
	{}

	UPROPERTY()
	TObjectPtr<const class UItemData> ItemData;

	UPROPERTY()
	int32 Quantity;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

public:
	UPROPERTY()
	TArray<FInventorySlot> Items;

	UPROPERTY()
	int32 MaxSlotCount = 20;

	UFUNCTION(BlueprintCallable)
	bool AddItem(const UItemData* ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(FName ItemID) const;
	
	UFUNCTION(BlueprintCallable)
	int32 RemoveItem(FName ItemID) const;

	//HealItem* GetEquippedHealItem() { return EquippedHealItem; }

	UPROPERTY()
	TObjectPtr<class HealItemData> EquippedHealItem;

protected:
	int32 FindStackableSlotIndex(const class UItemData* ItemData) const;

	int32 FindEmptySlotIndex() const;
};
