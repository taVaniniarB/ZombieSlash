// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "GameFramework/Character.h" 
#include "Interface/CharacterWeaponInterface.h"
#include "Interface/WeaponAnimInterface.h"
#include "Item/WeaponData.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	RootComponent = SceneComponent;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
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

	ACharacter * Character = GetOwner<ACharacter>();
	Character->GetMesh()->GetAnimInstance();
	IWeaponAnimInterface* Anim = Cast<IWeaponAnimInterface>(Character->GetMesh()->GetAnimInstance());
	Anim->SetCurWeaponType(WeaponData->WeaponType);
}

void AWeaponBase::OnUnequip()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}
