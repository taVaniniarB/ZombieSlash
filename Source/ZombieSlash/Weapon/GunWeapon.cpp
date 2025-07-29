// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Item/GunData.h"
#include "Engine/DamageEvents.h"
#include "Physics/ZSCollision.h"
#include "Interface/CharacterWeaponInterface.h"

AGunWeapon::AGunWeapon()
{
	SocketName = TEXT("Handgun_Socket");
}

void AGunWeapon::BeginPlay()
{
	Super::BeginPlay();

	const UGunData* Gun = GetGunData();
	CurAmmo = Gun->MaxAmmo;

	// ����->�Ѿ�, ũ�ν���� HUD CreateWidget, AddToViewport, Invisible
}

void AGunWeapon::OnEquip()
{
	Super::OnEquip();
	
	// Gun HUD Visible
	// ���� ī�޶� Ȱ��ȭ
}
void AGunWeapon::OnUnequip()
{
	Super::OnUnequip();
	// Gun HUD Invisible
	// ���� ī�޶� ��Ȱ
}
void AGunWeapon::StartAttack()
{
	Fire();
}

void AGunWeapon::Fire()
{
	UGameplayStatics::SpawnEmitterAttached(GetGunData()->MuzzleFlash, WeaponMeshComponent, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(GetGunData()->MuzzleSound, WeaponMeshComponent, TEXT("MuzzleFlashSocket"));

	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	if (!OwnerCharacter) return;

	AController* OwnerController = OwnerCharacter->GetWeaponOwnerController();
	if (!OwnerController) return;


	UAnimInstance* AnimInst = OwnerCharacter->GetWeaponOwnerAnimInstance();
	AnimInst->Montage_Play(GetGunData()->ShootMontage, 1.0f); // ����� ��Ÿ��, ��� �ӵ�


	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * (OwnerCharacter->GetWeaponOwnerStat().AttackRange);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, CCHANNEL_ZSACTION, QueryParams);

	if (bSuccess)
	{
		float Damage = OwnerCharacter->GetWeaponOwnerStat().Attack;

		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetGunData()->ImpactEffect, Hit.Location, ShotDirection.Rotation());

		AActor* Actor = Hit.GetActor();
		if (Actor)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			Actor->TakeDamage(Damage, DamageEvent, OwnerController, this);

			UE_LOG(LogTemp, Warning, TEXT("%s Hit"), *Actor->GetName());
		}
	}
}
