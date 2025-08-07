// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponSlotWidget.h"
#include "Inventory/WeaponSlot.h"

void UWeaponSlotWidget::NativeDestruct()
{
	// ���� ���� ���� ���� �� ���� �ε� �Լ� ȣ��
	if (OwnerInventory->bEquippedWeaponChanged)
	{
		OwnerInventory->LoadWeaponDataFromItems();
	}	
}