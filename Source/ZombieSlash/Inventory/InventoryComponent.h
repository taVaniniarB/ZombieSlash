// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/InventorySlot.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSlotChanged, const TArray<class UWeaponData*>, NewWeapons);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedInventory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWeaponSlotChanged OnWeaponSlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdatedInventory OnUpdatedInventory;


protected:
	virtual void BeginPlay() override;

	// 로드된 아이템 데이터를 캐싱하는 맵
	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UItemData>> ItemDataCache;
	// 비동기 로딩을 위한 콜백 함수
	void OnItemDataLoaded(FPrimaryAssetId ItemID);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventorySlot> Items;

	UPROPERTY()
	int32 MaxSlotCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurSlotCount = 0;

	// Weapon Slot Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//TArray<TObjectPtr<class WeaponData>> WeaponSlots; // 무기 슬롯
	TArray<FWeaponSlot> WeaponSlots; // 무기 슬롯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 WeaponSlotCount = 3;

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


public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(FPrimaryAssetId ItemID, int32 InQuantity);
	UFUNCTION(BlueprintCallable)
	void CreateNewStack(int32 Idx, FPrimaryAssetId ID, int32 Quantity);
	UFUNCTION(BlueprintCallable)
	virtual void TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCountByType(enum EItemType ItemType) const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCountByID(FPrimaryAssetId ID) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByID(FPrimaryAssetId ItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByIdx(int32 idx);

	// 인벤토리 내에서 아이템을 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	class UItemData* GetItemFromInventory(FPrimaryAssetId ItemID);
	// 캐시로부터 아이템 데이터를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UItemData* GetItemData(FPrimaryAssetId ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItemByID(FPrimaryAssetId ItemID, int32 UseQuantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItemByIndex(int32 Idx, int32 UseQuantity);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USoundBase> PickupSound;
	void PlayPickupSound();

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	
protected:
	int32 FindStackableSlotIndex(const class UItemData* ItemData) const;

	int32 FindEmptySlotIndex() const;

	int32 FindItemSlotIndexByID(FPrimaryAssetId InID) const;

	void InitializeItemDataCache();
};
