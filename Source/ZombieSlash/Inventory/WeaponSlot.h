// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "WeaponSlot.generated.h"

DECLARE_DELEGATE(FOnUpdatedWeaponSlot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UWeaponSlot : public UInventoryComponent
{
	GENERATED_BODY()

public:	
	UWeaponSlot();

	FOnUpdatedWeaponSlot OnUpdatedWeaponSlot;


	// 플레이어는 채워진 WeaponData를 바탕으로 액터를 생성 및 장착 프로세스를 진행한다


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<TObjectPtr<class UWeaponData>> WeaponData;

	UPROPERTY(EditAnywhere, Category = Weapon)
	uint8 bEquippedWeaponChanged : 1; // 이게 true면 인벤토리 닫을때 무기가 바뀌는 로직 호출

	UFUNCTION(BlueprintCallable)
	const UWeaponData* GetCurWeaponData(int32 InIndex) const;

	virtual void SetInventorySize(int32 InMaxSlotCount) override;

	void LoadWeaponDataFromItems(); // Items에 들어 있는 데이터를 Load하여 WeaponData를 세팅한다

protected:
	virtual void BeginPlay() override;

protected:
	virtual void TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory) override;
	void RegistWeaponData(class UItemData* ItemData, int32 Idx);

private:
	// 비동기 로딩 추적용 멤버 변수
	int32 PendingLoadsCount = 0;

};
