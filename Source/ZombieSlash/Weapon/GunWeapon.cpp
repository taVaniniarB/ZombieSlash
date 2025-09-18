// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Item/GunData.h"
#include "Engine/DamageEvents.h"
#include "Physics/ZSCollision.h"
#include "Interface/CharacterWeaponInterface.h"
#include "Inventory/InventoryComponent.h"
#include "Interface/CameraShakeInterface.h"
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
}

void AGunWeapon::OnEquip()
{
	Super::OnEquip();
	
	// �÷��̾� ĳ������ �κ��丮 ������Ʈ���� ź�� ���� ��������Ʈ ���ε�
	Inventory->OnAmmoChanged.AddUObject(this, &AGunWeapon::OnAmmoUpdated);

	// ���� ���� �� ���� ź�� ���� �ִ� ź�� ���� HUD�� �˸�
	OnAmmoChanged.Broadcast(CurAmmo, GetTotalAmmo());
}

void AGunWeapon::OnUnequip()
{
	Super::OnUnequip();

	// ���ε�� ��������Ʈ ����
	OnAmmoChanged.RemoveAll(GetOwner());
	Inventory->OnAmmoChanged.RemoveAll(this);
}

void AGunWeapon::OnAmmoUpdated(FPrimaryAssetId ChangedAmmoID, int32 NewQuantity)
{
	if (AmmoData && AmmoData->GetPrimaryAssetId() == ChangedAmmoID)
	{
		OnAmmoChanged.Broadcast(CurAmmo, NewQuantity);
	}
}

// �÷��̾� �Է¿� ���� ȣ��
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
	
	ShakeCamera();

	CurAmmo--;
	OnAmmoChanged.Broadcast(CurAmmo, GetTotalAmmo());

	// FireDelay�� �� bCanFire�� true�� �����
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, [this]()
		{
			bCanFire = true;
		}, GetGunData()->FireDelay, false);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		GetGunData()->MuzzleFlash, // ����� Niagara System ����
		WeaponMeshComponent,       // ������ ������Ʈ
		TEXT("MuzzleFlashSocket"), // ������ ���� �̸�
		FVector::ZeroVector,       // ��� ��ġ ������ (���� ����)
		FRotator::ZeroRotator,     // ��� ȸ�� ������ (���� ����)
		EAttachLocation::SnapToTarget, // ���� ��ġ�� ����
		true                       // �ڵ����� Ȱ��ȭ (��� ����)
	);
	
	UGameplayStatics::SpawnSoundAttached(GetGunData()->MuzzleSound, WeaponMeshComponent, TEXT("MuzzleFlashSocket"));

	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	if (!OwnerCharacter) return;

	AController* OwnerController = OwnerCharacter->GetWeaponOwnerController();
	if (!OwnerController) return;

	UAnimInstance* AnimInst = OwnerCharacter->GetWeaponOwnerAnimInstance();
	
	if (GetGunData()->ShootMontage)
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
		float Damage = OwnerCharacter->GetWeaponOwnerStat().Attack * 0.5;

		FVector ShotDirection = -Rotation.Vector();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),                   
			GetGunData()->ImpactEffect,   
			Hit.Location,                   // ����Ʈ ���� ��ġ
			ShotDirection.Rotation()        // ����Ʈ�� ȸ��
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
	// ���� ��Ÿ�� �÷���
	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	
	if (!OwnerCharacter || !GetGunData()->ReloadMontage)
		return;
	
	UAnimInstance* Anim = OwnerCharacter->GetWeaponOwnerAnimInstance();

	Anim->Montage_Play(GetGunData()->ReloadMontage, 1.0f);
	
	// ��Ÿ�� ���� ��������Ʈ ���ε�
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGunWeapon::OnReloadMontageEnded);
	Anim->Montage_SetEndDelegate(EndDelegate, GetGunData()->ReloadMontage);
}

bool AGunWeapon::CanReload()
{
	if (!Inventory) return false;

	int32 TotalAmmo = Inventory->GetItemCountByID(GetGunData()->AmmoData->GetPrimaryAssetId());
	
	if (TotalAmmo <= 0) return false;

	int32 ToCharge = GetGunData()->MaxAmmo - CurAmmo;

	if (ToCharge <= 0) // źâ�� �� ��
	{
		UE_LOG(LogTemp, Warning, TEXT("Full Ammo"));
		return false;
	}

	return true;
}

void AGunWeapon::OnReloadMontageEnded(class UAnimMontage* Montage, bool bInterrupted)
{
	// �Է� Ȱ��ȭ
	ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->EndReload();
	}
	
	if (bInterrupted)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnReloadMontageEnded Interrupted"));
		return;
	}

	if (!Inventory) return;

	// ä�� �Ѿ��� ����
	int32 MaxAmmo = GetGunData()->MaxAmmo;
	int32 ToCharge = MaxAmmo - CurAmmo;
	int32 TotalAmmo = GetTotalAmmo();

	if (TotalAmmo < ToCharge) // ä���� �ϴ� �Ѿ˺��� �κ��丮 �� �Ѿ��� ���ٸ� �κ��丮 �� �Ѿ��� ������ŭ�� ����
	{
		ToCharge = TotalAmmo;
	}

	CurAmmo += ToCharge; // źâ ä���
	
	if (Inventory->UseItemByID(AmmoData->GetPrimaryAssetId(), ToCharge))
	{
		OnAmmoChanged.Broadcast(CurAmmo, FMath::Max(0, TotalAmmo - ToCharge)); // UI�� �˸�
	}
}

int32 AGunWeapon::GetTotalAmmo()
{
	if (!Inventory) return 0;

	return Inventory->GetItemCountByID(GetGunData()->AmmoData->GetPrimaryAssetId());
}

void AGunWeapon::ShakeCamera()
{
	/*ICameraShakeInterface* Pawn = Cast<ICameraShakeInterface>(GetOwner());
	if (Pawn)
	{
		Pawn->ShakeCamera(CameraShakeType, 1.0f);
	}*/
}
