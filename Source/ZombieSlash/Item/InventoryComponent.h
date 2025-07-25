// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot(UItemData* InItemData, int32 InQuantity)
		: ItemData(InItemData), Quantity(InQuantity)
	{}

	FInventorySlot()
		: ItemData(nullptr), Quantity(0)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UItemData> ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Quantity;

public:
	bool IsValid()
	{
		if (ItemData && Quantity > 0) return true;
		return false;
	}
};

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWeaponData> WeaponData; // 무기 데이터

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWeaponBase> WeaponActor; // 스폰된 무기 액터
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSlotChanged, const class UWeaponData*, NewWeapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	FOnWeaponSlotChanged OnWeaponSlotChanged;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventorySlot> Items;

	UPROPERTY()
	int32 MaxSlotCount = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurSlotCount = 0;

	// Weapon Slot Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//TArray<TObjectPtr<class WeaponData>> WeaponSlots; // 무기 슬롯
	TArray<FWeaponSlot> WeaponSlots; // 무기 슬롯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 WeaponSlotCount = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 CurWeaponSlotIdx = 0; // 현재 장착된 슬롯 인덱스
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	uint8 bEquippedWeaponChanged : 1; // 이게 true면 인벤토리 닫을때 무기가 바뀌는 로직 호출

	UFUNCTION(BlueprintCallable)
	bool SwitchWeapon(int32 SlotIdx);

	UFUNCTION(BlueprintCallable)
	bool EquipWeapon(int32 InventoryIndex, int32 SlotNumber);

	UFUNCTION(BlueprintCallable)
	const UWeaponData* GetCurWeaponData() const;

	UFUNCTION(BlueprintCallable)
	void InitializeWeaponSlots();


	// Inventory Logic
public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemData* ItemData, int32 Quantity);

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
