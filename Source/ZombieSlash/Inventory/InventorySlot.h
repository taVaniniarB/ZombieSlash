#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData.h"
#include "InventorySlot.generated.h" 


USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot(FPrimaryAssetId InItemID, int32 InQuantity, EItemType InItemType)
		: ItemID(InItemID), Quantity(InQuantity)
	{
	}

	FInventorySlot()
		: ItemID(), Quantity(0)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetId ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Quantity;

public:
	bool IsValid() const
	{
		return ItemID.IsValid() && Quantity > 0;
	}
};

//USTRUCT(BlueprintType)
//struct FWeaponSlot
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TObjectPtr<class UWeaponData> WeaponData; // 무기 데이터
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	TObjectPtr<class AWeaponBase> WeaponActor; // 스폰된 무기 액터
//};