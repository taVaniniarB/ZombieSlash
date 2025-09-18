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
	// ���� �����
	// �״� �ִϸ��̼��� ������ ������ Attach�� ����, ��ü�� �����ϰ�, ��ġ, ���� ������ Pickup ������

	AWeaponBase* Weapon = WeaponManager->GetCurrentWeapon();

	// ���� ���� �Ǵ� �޽��� ��ġ ��������
	FVector WeaponLocation;
	FRotator WeaponRotation;
	UStaticMesh* WeaponMesh = nullptr;

	UStaticMeshComponent* MeshComponent = Weapon->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent) {
		WeaponMesh = MeshComponent->GetStaticMesh();
	}

	FName SocketName = Weapon->GetSocketName();
	// ������ �ִ� ��� ���� ��ġ ���
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
	if (WeaponPickup)
	{
		UE_LOG(LogTemp, Log, TEXT("Boss weapon pickup created successfully"));
	}
}