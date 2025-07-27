// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Item/GunData.h"
#include "Engine/DamageEvents.h"
#include "Physics/ZSCollision.h"
#include "Interface/CharacterWeaponInterface.h"

void AGunWeapon::BeginPlay()
{
	Super::BeginPlay();

	const UGunData* Gun = GetGunData();
	MaxRange = Gun->MaxRange;
	MaxAmmo = Gun->MaxAmmo;
	CurAmmo = MaxAmmo;
}

void AGunWeapon::OnEquip()
{
	Super::OnEquip();
	// 플컨->총알, 크로스헤어 HUD CreateWidget, AddToViewport

	// 소켓과 본 위치 조정
	// 에임 카메라 비활성화
}
void AGunWeapon::OnUnequip()
{
	Super::OnUnequip();
	// 플컨->총알, 크로스헤어 HUD CreateWidget, AddToViewport
}
void AGunWeapon::StartAttack()
{
	Fire();
}

void AGunWeapon::Fire()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMeshComponent, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, WeaponMeshComponent, TEXT("MuzzleFlashSocket"));

	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());

	if (!OwnerCharacter) return;

	AController* OwnerController = OwnerCharacter->GetWeaponOwnerController();
	if (!OwnerController) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, CCHANNEL_ZSACTION);

	if (bSuccess)
	{
		float Damage = OwnerCharacter->GetWeaponOwnerStat().Attack;

		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());

		AActor* Actor = Hit.GetActor();
		if (Actor)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			Actor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}
