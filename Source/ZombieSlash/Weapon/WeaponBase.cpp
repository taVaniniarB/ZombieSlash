// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Interface/CharacterWeaponInterface.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	SocketName = TEXT("hand_rSocket");

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMeshComponent;
}

void AWeaponBase::InitializeWeapon(UWeaponData* InWeaponData, AActor* InOwner)
{
	WeaponData = InWeaponData;
	SetOwner(InOwner);
}

void AWeaponBase::OnEquip()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
}

void AWeaponBase::OnUnequip()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}
