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


	// �÷��̾�� ä���� WeaponData�� �������� ���͸� ���� �� ���� ���μ����� �����Ѵ�


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<TObjectPtr<class UWeaponData>> WeaponData;

	UPROPERTY(EditAnywhere, Category = Weapon)
	uint8 bEquippedWeaponChanged : 1; // �̰� true�� �κ��丮 ������ ���Ⱑ �ٲ�� ���� ȣ��

	UFUNCTION(BlueprintCallable)
	const UWeaponData* GetCurWeaponData(int32 InIndex) const;

	virtual void SetInventorySize(int32 InMaxSlotCount) override;

	void LoadWeaponDataFromItems(); // Items�� ��� �ִ� �����͸� Load�Ͽ� WeaponData�� �����Ѵ�

protected:
	virtual void BeginPlay() override;

protected:
	virtual void TransferSlot(int32 DestIdx, int32 SrcIdx, UInventoryComponent* SrcInventory) override;
	void RegistWeaponData(class UItemData* ItemData, int32 Idx);

private:
	// �񵿱� �ε� ������ ��� ����
	int32 PendingLoadsCount = 0;

};
