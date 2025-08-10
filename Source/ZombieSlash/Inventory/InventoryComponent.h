// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySlot.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedInventory);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryAmmoChanged, FPrimaryAssetId /*AmmoID*/, int32 /*NewQuantity*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdatedInventory OnUpdatedInventory;

	FOnInventoryAmmoChanged OnAmmoChanged;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlot> Items;

	UPROPERTY()
	int32 MaxSlotCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurSlotCount = 0;

	UFUNCTION(BlueprintCallable)
	virtual void SetInventorySize(int32 InMaxSlotCount);

public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(FPrimaryAssetId ItemID, int32 InQuantity);
	
	UFUNCTION(BlueprintCallable)
	void CreateNewStack(int32 Idx, FPrimaryAssetId ID, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	virtual void TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCountByID(FPrimaryAssetId ID) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByID(FPrimaryAssetId ItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByIdx(int32 idx);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItemByID(FPrimaryAssetId ItemID, int32 UseQuantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItemByIndex(int32 Idx, int32 UseQuantity);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USoundBase> PickupSound;
	void PlayPickupSound();

	UFUNCTION(BlueprintCallable)
	virtual void UpdateInventory();

	
protected:
	int32 FindEmptySlotIndex() const;

	int32 FindItemSlotIndexByID(FPrimaryAssetId InID) const;

private:
	bool TryGetItemMetadata(FPrimaryAssetId ItemID, struct FItemMetadata& OutMetadata);
	int32 AddToExistingStacks(FPrimaryAssetId ItemID, int32 Quantity, int32 MaxStack);
	bool AddToNewStacks(FPrimaryAssetId ItemID, int32 Quantity, int32 MaxStack);
	void BroadcastAmmoChanged(FPrimaryAssetId ItemID);
};
