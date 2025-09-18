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
	// 무기 떼어내기
	// 죽는 애니메이션이 끝나는 시점에 Attach를 뗴어내고, 객체를 삭제하고, 위치, 각도 동일한 Pickup 만들자

	AWeaponBase* Weapon = WeaponManager->GetCurrentWeapon();

	// 무기 소켓 또는 메시의 위치 가져오기
	FVector WeaponLocation;
	FRotator WeaponRotation;
	UStaticMesh* WeaponMesh = nullptr;

	UStaticMeshComponent* MeshComponent = Weapon->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent) {
		WeaponMesh = MeshComponent->GetStaticMesh();
	}

	FName SocketName = Weapon->GetSocketName();
	// 소켓이 있는 경우 소켓 위치 사용
	if (GetMesh()->DoesSocketExist(SocketName))
	{
		WeaponLocation = GetMesh()->GetSocketLocation(SocketName);
		WeaponRotation = GetMesh()->GetSocketRotation(SocketName);
	}

	// Pickup 생성
	AItemPickup* WeaponPickup = AItemPickup::CreatePickup(
		GetWorld(),
		WeaponLocation,
		WeaponRotation,
		Weapon->GetWeaponData()->GetPrimaryAssetId(),
		1, // 수량
		WeaponMesh
	);
	if (WeaponPickup)
	{
		UE_LOG(LogTemp, Log, TEXT("Boss weapon pickup created successfully"));
	}
}