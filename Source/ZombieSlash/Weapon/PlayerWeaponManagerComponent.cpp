// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponManagerComponent.h"
#include "Inventory/WeaponSlot.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/GunWeapon.h"
#include "Interface/WeaponAnimInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterPlayer.h"
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h" 

UPlayerWeaponManagerComponent::UPlayerWeaponManagerComponent()
{
	WeaponSlotCount = 3;
	CurWeaponIndex = 0;
}

void UPlayerWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetPlayerOwner();
}

void UPlayerWeaponManagerComponent::EquipWeaponByIndex(int32 Index)
{
	Super::EquipWeaponByIndex(Index);

	SetGunState(EGunState::Ready, false, false);
	AWeaponBase* CurWeapon = GetCurrentWeapon();

	UpdateWeaponIMC(CurWeapon->GetWeaponType());
	if (CurWeapon->GetWeaponType() == EWeaponType::Gun)
	{
		AGunWeapon* GunWeapon = Cast<AGunWeapon>(CurWeapon);
		if (GunWeapon)
		{
			// HUD 업데이트를 위한 델리게이트 바인딩
			GunWeapon->OnAmmoChanged.AddUObject(PlayerOwner, &ACharacterPlayer::HandleAmmoChanged);
		}
	}

	CurWeapon->OnEquip();
}

void UPlayerWeaponManagerComponent::Attack()
{
	Super::Attack();
	
	// 총기일 경우 조준 상태로 전환
	if (EWeaponType::Gun == Weapons[CurWeaponIndex]->GetWeaponType())
	{
		EnterAimState();

		IWeaponAnimInterface* Anim = Cast<IWeaponAnimInterface>(PlayerOwner->GetMesh()->GetAnimInstance());
		Anim->Shoot();
	}
}


void UPlayerWeaponManagerComponent::SwitchWeapon(const FInputActionInstance& Value)
{
   const float ScrollValue = Value.GetValue().Get<float>();
   if (FMath::IsNearlyZero(ScrollValue) || !PlayerOwner)
       return;

   UWeaponSlot* WeaponSlot = PlayerOwner->GetWeaponSlot();
   if (!WeaponSlot)
       return;

   // 플레이어 상태 체크 (줌, 루트 모션 등)
   if (PlayerOwner->IsZooming() || PlayerOwner->IsPlayingRootMotion())
       return;

   int32 TotalSlots = WeaponSlot->MaxSlotCount;
   if (TotalSlots == 0)
       return;

   int32 Direction = ScrollValue > 0 ? 1 : -1;

   // 다음 유효한 무기를 찾고 현재 무기를 가리킬 인덱스를 세팅
   int32 NewIndex = CurWeaponIndex;
   for (int32 i = 0; i < TotalSlots; ++i)
   {
       NewIndex = (NewIndex + Direction + TotalSlots) % TotalSlots;
       if (Weapons.IsValidIndex(NewIndex) && Weapons[NewIndex])
       {
           break;
       }
   }

   // 인덱스가 변경되지 않았으면 무기 교체 X
   if (NewIndex == CurWeaponIndex)
   {
       return;
   }

   // 이전 무기 해제
   AWeaponBase* PrevWeapon = GetCurrentWeapon();
   if (PrevWeapon)
   {
       PrevWeapon->OnUnequip();
   }

   // 새 무기 장착
   CurWeaponIndex = NewIndex;
   EquipWeaponByIndex(NewIndex);
}

void UPlayerWeaponManagerComponent::EnterAimState()
{
	if (PlayerOwner)
	{
		SetGunState(EGunState::Aim, PlayerOwner->IsZooming(), false);
		PlayerOwner->ResetExitAimTimer();
	}
}

void UPlayerWeaponManagerComponent::SetGunState(EGunState State, uint8 InIsZooming, bool PlayMontage)
{
	if (PlayerOwner)
	{
		PlayerOwner->SetGunState(State, InIsZooming, PlayMontage);
	}
}

void UPlayerWeaponManagerComponent::UpdateWeaponIMC(EWeaponType WeaponType)
{
	if (PlayerOwner)
	{
		PlayerOwner->UpdateWeaponIMC(WeaponType);
	}
}

void UPlayerWeaponManagerComponent::SetPlayerOwner()
{
	if (!PlayerOwner)
	{
		PlayerOwner = Cast<ACharacterPlayer>(GetOwner());
	}
}
