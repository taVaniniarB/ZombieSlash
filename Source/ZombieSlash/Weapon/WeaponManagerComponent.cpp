// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponManagerComponent.h"
#include "WeaponManagerComponent.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/GunWeapon.h"
#include "Item/WeaponData.h"
#include "Inventory/WeaponSlot.h"
#include "Character/CharacterBase.h"
#include "CharacterStat/CharacterStatComponent.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	WeaponSlotCount = 1;
	CurWeaponIndex = 0;
}

// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetOwnerCharacter();

	Weapons.SetNum(WeaponSlotCount);
	UWeaponSlot* WeaponSlot = OwnerCharacter->GetWeaponSlot();
	WeaponSlot->SetInventorySize(WeaponSlotCount);
	WeaponSlot->OnUpdatedWeaponSlot.BindUObject(this, &UWeaponManagerComponent::InitializeWeapons);
	WeaponSlot->LoadWeaponDataFromItems();

	//InitializeWeapons();
}

void UWeaponManagerComponent::InitializeWeapons()
{
	if (!OwnerCharacter) return;
	UWeaponSlot* WeaponSlot = OwnerCharacter->GetWeaponSlot();
	if (!WeaponSlot) return;

	// WeaponSlot 내 WeaponData를 참조하여 무기 액터를 생성하고, 이를 Weapons 배열에 보관한다.
	for (int32 i = 0; i < WeaponSlotCount; ++i)
	{
		UWeaponData* NewWeaponData = WeaponSlot->WeaponData[i];

		// i 슬롯에 무기 데이터가 없다면 해당 슬롯의 무기를 제거
		if (!NewWeaponData)
		{
			if (Weapons.IsValidIndex(i) && Weapons[i])
			{
				Weapons[i]->OnUnequip();
				Weapons[i]->Destroy();
				Weapons[i] = nullptr;
			}
			continue;
		}

		// 기존 무기와 동일한 데이터라면 유지
		if (Weapons.IsValidIndex(i) && Weapons[i])
		{
			if (Weapons[i]->GetWeaponData() == NewWeaponData)
				continue;

			// 다른 데이터라면 제거
			Weapons[i]->OnUnequip();
			Weapons[i]->Destroy();
			Weapons[i] = nullptr;
		}


		// 새 무기 액터 생성
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;

		AWeaponBase* WeaponInst = GetWorld()->SpawnActor<AWeaponBase>(NewWeaponData->WeaponActorClass, SpawnParams);
		if (!WeaponInst)
		{
			UE_LOG(LogTemp, Error, TEXT("Weapon spawn failed at slot %d"), i);
			continue;
		}

		WeaponInst->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponInst->GetSocketName());
		WeaponInst->InitializeWeapon(NewWeaponData, OwnerCharacter);
		WeaponInst->OnUnequip(); // 스폰된 무기를 Invisible하게 만들 목적

		Weapons[i] = WeaponInst;
	}

	EquipWeaponByIndex(CurWeaponIndex);

	WeaponSlot->bEquippedWeaponChanged = false;
}

void UWeaponManagerComponent::EquipWeaponByIndex(int32 Index)
{
	if (!OwnerCharacter) return;
	UWeaponSlot* WeaponSlot = OwnerCharacter->GetWeaponSlot();

	if (!Weapons.IsValidIndex(Index) || !Weapons[Index]) return;

	AWeaponBase* CurWeapon = Weapons[Index];
	if (!Weapons[Index])
		UE_LOG(LogTemp, Error, TEXT("No weapon in slot %d"), Index);
	
	UCharacterStatComponent* StatComp = OwnerCharacter->FindComponentByClass<UCharacterStatComponent>();
	if (StatComp)
	{
		StatComp->SetModifierStat(WeaponSlot->WeaponData[Index]->ModifierStat);
	}

	CurWeapon->OnEquip();

	OnWeaponEquipped.Broadcast(CurWeapon->GetWeaponType());
}

bool UWeaponManagerComponent::Reload()
{
	if (EWeaponType::Gun != Weapons[CurWeaponIndex]->GetWeaponType()) return false;

	AGunWeapon* Gun = Cast<AGunWeapon>(Weapons[CurWeaponIndex]);

	if (Gun->CanReload() && !(OwnerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()))
	{
		Gun->Reload();
		return true;
	}

	return false;
}

void UWeaponManagerComponent::Attack()
{
	AWeaponBase* CurrentWeapon = GetCurrentWeapon();
	if (!CurrentWeapon || !CanAttack())
	{
		return;
	}

	CurrentWeapon->StartAttack();
}

bool UWeaponManagerComponent::CanAttack() const
{
	AWeaponBase* CurrentWeapon = GetCurrentWeapon();
	return CurrentWeapon && CurrentWeapon->CanAttack();
}

AWeaponBase* UWeaponManagerComponent::GetCurrentWeapon() const
{
	return Weapons[CurWeaponIndex];
}

EWeaponType UWeaponManagerComponent::GetCurrentWeaponType() const
{
	AWeaponBase* CurrentWeapon = GetCurrentWeapon();
	return CurrentWeapon->GetWeaponType();
}

void UWeaponManagerComponent::AttachWeaponToOwner(AWeaponBase* Weapon)
{
	if (!Weapon || !OwnerCharacter)
	{
		return;
	}

	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if (OwnerMesh)
	{
		Weapon->AttachToComponent(OwnerMesh,
			FAttachmentTransformRules::KeepRelativeTransform,
			Weapon->GetSocketName());
	}
}

void UWeaponManagerComponent::SetOwnerCharacter()
{
	OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponComponent owner is not ACharacterBase"));
	}
}
