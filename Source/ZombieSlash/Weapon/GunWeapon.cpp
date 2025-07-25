// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Item/GunData.h"
#include "Engine/DamageEvents.h"
#include "Physics/ZSCollision.h"



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
	/*UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	if (!OwnerCharacter) return;

	AController* OwnerController = OwnerCharacter->GetController();
	if (!OwnerController) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, CCHANNEL_ZSACTION);

	if (bSuccess)
	{
		float Damage = OwningPlayer->GetTotalStat().Attack;

		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());

		AActor* Actor = Hit.GetActor();
		if (Actor)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			Actor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}*/
}
