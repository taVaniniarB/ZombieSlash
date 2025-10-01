// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBoss.h"
#include "Weapon/WeaponManagerComponent.h"
#include "Inventory/WeaponSlot.h"
#include "Weapon/WeaponBase.h"
#include "Item/WeaponData.h"
#include "Item/ItemPickup.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"
#include "AI/BossAIController.h"

ACharacterBoss::ACharacterBoss()
{
	SetCharacterID(TEXT("Enemy_Boss"));
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	
	DetectAngle = 70.0f;
	DetectRange = 500.0f;
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

void ACharacterBoss::AttackByAI()
{
	UE_LOG(LogTemp, Warning, TEXT("ACharacterBoss::AttackByAI"));
	UpdateWarpTarget();
	Super::AttackByAI();
}

void ACharacterBoss::UpdateWarpTarget()
{
	if (!MotionWarpingComp) return;

	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController) return;
	UObject* Target = BossController->GetTarget();
	if (!Target) return;
	AActor* TargetPawn = Cast<AActor>(Target);
	if (!TargetPawn) return;

	FVector TargetLocation = TargetPawn->GetActorLocation();
	FRotator TargetRotation = FRotator(0.0f, (TargetPawn->GetActorLocation() - GetActorLocation()).Rotation().Yaw, 0.0f);
	MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("AttackPoint"),
		TargetLocation,
		TargetRotation
	);
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