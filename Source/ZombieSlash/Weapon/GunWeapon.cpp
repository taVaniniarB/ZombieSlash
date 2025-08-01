// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Item/GunData.h"
#include "Engine/DamageEvents.h"
#include "Physics/ZSCollision.h"
#include "Interface/CharacterWeaponInterface.h"
#include "Item/InventoryComponent.h"
#include "Item/AmmoItemData.h"

AGunWeapon::AGunWeapon()
{
	SocketName = TEXT("Handgun_Socket");
	bCanFire = true;
}

void AGunWeapon::BeginPlay()
{
	Super::BeginPlay();

	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());

	if(OwnerCharacter)
	{
		Inventory = OwnerCharacter->GetInventory();
		if (!Inventory)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory was null"));
			return;
		}
	}

	const UGunData* Gun = GetGunData();
	CurAmmo = Gun->MaxAmmo;
	AmmoData = Gun->AmmoData;

	// 플컨->총알, 크로스헤어 HUD CreateWidget, AddToViewport, Invisible
}

void AGunWeapon::OnEquip()
{
	Super::OnEquip();
	
	// Gun HUD Visible
}
void AGunWeapon::OnUnequip()
{
	Super::OnUnequip();
	// Gun HUD Invisible
}

// 플레이어 입력에 의해 호출
void AGunWeapon::StartAttack()
{
	if (bCanFire && CurAmmo > 0)
	{
		Fire();
	}
}

void AGunWeapon::Fire()
{
	bCanFire = false;
	CurAmmo--;

	// FireDelay초 후 bCanFire를 true로 만든다
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, [this]()
		{
			bCanFire = true;
		}, GetGunData()->FireDelay, false);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		GetGunData()->MuzzleFlash, // 변경된 Niagara System 에셋
		WeaponMeshComponent,       // 부착할 컴포넌트
		TEXT("MuzzleFlashSocket"), // 부착할 소켓 이름
		FVector::ZeroVector,       // 상대 위치 오프셋 (소켓 기준)
		FRotator::ZeroRotator,     // 상대 회전 오프셋 (소켓 기준)
		EAttachLocation::SnapToTarget, // 소켓 위치에 스냅
		true                       // 자동으로 활성화 (재생 시작)
	);
	
	UGameplayStatics::SpawnSoundAttached(GetGunData()->MuzzleSound, WeaponMeshComponent, TEXT("MuzzleFlashSocket"));

	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	if (!OwnerCharacter) return;

	AController* OwnerController = OwnerCharacter->GetWeaponOwnerController();
	if (!OwnerController) return;

	UAnimInstance* AnimInst = OwnerCharacter->GetWeaponOwnerAnimInstance();
	
	if (GetGunData()->ShootMontage)
		AnimInst->Montage_Play(GetGunData()->ShootMontage, 1.0f); // 재생할 몽타주, 재생 속도

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
		float Damage = OwnerCharacter->GetWeaponOwnerStat().Attack * 0.5;

		FVector ShotDirection = -Rotation.Vector();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),                   
			GetGunData()->ImpactEffect,   
			Hit.Location,                   // 이펙트 스폰 위치
			ShotDirection.Rotation()        // 이펙트의 회전
		);
		
		AActor* Actor = Hit.GetActor();
		if (Actor)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			Actor->TakeDamage(Damage, DamageEvent, OwnerController, this);

			UE_LOG(LogTemp, Warning, TEXT("%s Hit"), *Actor->GetName());
		}
	}
}

void AGunWeapon::Reload()
{
	// 장전 몽타주 플레이
	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	
	if (!OwnerCharacter || !GetGunData()->ReloadMontage)
		return;
	
	UAnimInstance* Anim = OwnerCharacter->GetWeaponOwnerAnimInstance();

	Anim->Montage_Play(GetGunData()->ReloadMontage, 1.0f);
	
	// 몽타주 종료 델리게이트 바인드
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGunWeapon::OnReloadMontageEnded);
	Anim->Montage_SetEndDelegate(EndDelegate, GetGunData()->ReloadMontage);
}

bool AGunWeapon::CanReload()
{
	int32 TotalAmmo = Inventory->GetItemCountByID(GetGunData()->AmmoData->GetPrimaryAssetId());
	
	if (TotalAmmo <= 0) return false;

	int32 ToCharge = GetGunData()->MaxAmmo - CurAmmo;

	if (ToCharge <= 0) // 탄창이 꽉 참
	{
		UE_LOG(LogTemp, Warning, TEXT("Full Ammo"));
		return false;
	}

	return true;
}

void AGunWeapon::OnReloadMontageEnded(class UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("OnReloadMontageEnded Called"));
	// 입력 활성화
	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->EndReload();
	}
	
	if (bInterrupted)
	{
		return;
	}

	// 채울 총알의 개수
	int32 ToCharge = GetGunData()->MaxAmmo - CurAmmo;
	int32 TotalAmmo = Inventory->GetItemCountByID(GetGunData()->AmmoData->GetPrimaryAssetId());

	if (TotalAmmo < ToCharge) // 채워야 하는 총알보다 인벤토리 내 총알이 적다면 인벤토리 내 총알의 개수만큼만 장전
	{
		ToCharge = TotalAmmo;
	}

	CurAmmo += ToCharge; // 탄창 채우기
	
	Inventory->UseItem(AmmoData->GetPrimaryAssetId(), ToCharge);
}
