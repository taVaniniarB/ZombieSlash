// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

//#include "ItemPickup.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<const class UItemData> ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Quantity;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventorySlot> Items;

	UPROPERTY()
	int32 MaxSlotCount = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurSlotCount = 0;

	UPROPERTY()
	int32 WeaponSlotCount = 2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInventorySlot EquippedWeaponSlot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInventorySlot EquippedWeaponSlot2;

	//UPROPERTY()
	//TArray<TObjectPtr<const class UWeaponData>> EquippedWeapons;

	//UFUNCTION()
	//const TArray<TObjectPtr<const class UWeaponData>>& GetEquippedWeapons() const;

	UFUNCTION(BlueprintCallable)
	bool EquipWeapon(int32 InventoryIndex, int32 SlotNumber);

	UFUNCTION(BlueprintCallable)
	bool AddItem(const UItemData* ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(FName ItemID) const;

	UFUNCTION(BlueprintCallable)
	int32 GetItemCountByType(enum EItemType ItemType) const;
	
	UFUNCTION(BlueprintCallable)
	int32 RemoveItem(FName ItemID);

	UFUNCTION(BlueprintCallable)
	bool UseItem(FName ItemID);

	UPROPERTY()
	FName CurHealItemID;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetCurHealItemID() const { return CurHealItemID; }

	
protected:
	int32 FindStackableSlotIndex(const class UItemData* ItemData) const;

	int32 FindEmptySlotIndex() const;

	int32 FindItemSlotIndexByID(FName InID) const;
};
