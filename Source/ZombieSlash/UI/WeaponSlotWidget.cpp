// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponSlotWidget.h"
#include "Inventory/WeaponSlot.h"

void UWeaponSlotWidget::NativeDestruct()
{
	// 무기 슬롯 변동 있을 시 무기 로드 함수 호출
	if (OwnerInventory->bEquippedWeaponChanged)
	{
		OwnerInventory->LoadWeaponDataFromItems();
	}	
}