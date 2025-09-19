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

	// WeaponSlot �� WeaponData�� �����Ͽ� ���� ���͸� �����ϰ�, �̸� Weapons �迭�� �����Ѵ�.
	for (int32 i = 0; i < WeaponSlotCount; ++i)
	{
		UWeaponData* NewWeaponData = WeaponSlot->WeaponData[i];

		// i ���Կ� ���� �����Ͱ� ���ٸ� �ش� ������ ���⸦ ����
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

		// ���� ����� ������ �����Ͷ�� ����
		if (Weapons.IsValidIndex(i) && Weapons[i])
		{
			if (Weapons[i]->GetWeaponData() == NewWeaponData)
				continue;

			// �ٸ� �����Ͷ�� ����
			Weapons[i]->OnUnequip();
			Weapons[i]->Destroy();
			Weapons[i] = nullptr;
		}


		// �� ���� ���� ����
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
		WeaponInst->OnUnequip(); // ������ ���⸦ Invisible�ϰ� ���� ����

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
