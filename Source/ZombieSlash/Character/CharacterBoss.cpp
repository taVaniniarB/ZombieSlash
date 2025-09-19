// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBoss.h"
#include "Weapon/WeaponManagerComponent.h"
#include "Inventory/WeaponSlot.h"
#include "Weapon/WeaponBase.h"
#include "Item/WeaponData.h"
#include "Item/ItemPickup.h"

ACharacterBoss::ACharacterBoss()
{
	SetCharacterID(TEXT("Enemy_Boss"));
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
}

void ACharacterBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBoss::SetDead()
{
	Super::SetDead();

	UE_LOG(LogTemp, Warning, TEXT("Boss::SetDead"));
}

void ACharacterBoss::OnBossDead()
{
	UE_LOG(LogTemp, Warning, TEXT("BossDead"));

	// �״� �ִϸ��̼��� ������ ������
	// 1. ������ ��� �ִ� ����� ��ġ, ������ ������ Pickup�� �����
	// 2. ���� ��ü�� �����Ѵ�

	AWeaponBase* Weapon = WeaponManager->GetCurrentWeapon();
	
	UStaticMeshComponent* MeshComponent = Weapon->FindComponentByClass<UStaticMeshComponent>();
	if (!MeshComponent) return;

	FVector WeaponLocation = MeshComponent->GetComponentLocation();
	FRotator WeaponRotation = MeshComponent->GetComponentRotation();

	UStaticMesh* WeaponMesh = nullptr;

	WeaponMesh = MeshComponent->GetStaticMesh();

	// ���� ��ġ ���
	FName SocketName = Weapon->GetSocketName();
	if (GetMesh()->DoesSocketExist(SocketName))
	{
		WeaponLocation = GetMesh()->GetSocketLocation(SocketName);
		WeaponRotation = GetMesh()->GetSocketRotation(SocketName);
	}

	// Pickup ����
	AItemPickup* WeaponPickup = AItemPickup::CreatePickup(
		GetWorld(),
		WeaponLocation,
		WeaponRotation,
		Weapon->GetWeaponData()->GetPrimaryAssetId(),
		1, // ����
		WeaponMesh
	);

	// ���� BP�� ������ �ݿ�
	UStaticMeshComponent* PickupMesh = WeaponPickup->FindComponentByClass<UStaticMeshComponent>();
	if (PickupMesh)
	{
		// ���� ���� �޽� �������� ����
		UStaticMeshComponent* WeaponMeshComp = Weapon->FindComponentByClass<UStaticMeshComponent>();
		if (WeaponMeshComp)
		{
			PickupMesh->SetRelativeTransform(WeaponMeshComp->GetRelativeTransform());
		}
	}

	// ���� ����
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->Destroy();
}