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

	// 죽는 애니메이션이 끝나는 시점에
	// 1. 보스가 들고 있던 무기와 위치, 각도가 동일한 Pickup을 만든다
	// 2. 무기 객체를 삭제한다

	AWeaponBase* Weapon = WeaponManager->GetCurrentWeapon();
	
	UStaticMeshComponent* MeshComponent = Weapon->FindComponentByClass<UStaticMeshComponent>();
	if (!MeshComponent) return;

	FVector WeaponLocation = MeshComponent->GetComponentLocation();
	FRotator WeaponRotation = MeshComponent->GetComponentRotation();

	UStaticMesh* WeaponMesh = nullptr;

	WeaponMesh = MeshComponent->GetStaticMesh();

	// 소켓 위치 사용
	FName SocketName = Weapon->GetSocketName();
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

	// 무기 BP의 오프셋 반영
	UStaticMeshComponent* PickupMesh = WeaponPickup->FindComponentByClass<UStaticMeshComponent>();
	if (PickupMesh)
	{
		// 원래 무기 메시 오프셋을 복사
		UStaticMeshComponent* WeaponMeshComp = Weapon->FindComponentByClass<UStaticMeshComponent>();
		if (WeaponMeshComp)
		{
			PickupMesh->SetRelativeTransform(WeaponMeshComp->GetRelativeTransform());
		}
	}

	// 무기 삭제
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->Destroy();
}