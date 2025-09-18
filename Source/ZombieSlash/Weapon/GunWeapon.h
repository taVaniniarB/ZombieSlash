// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Item/GunData.h"
#include "Enums/CameraShakeType.h"
#include "GunWeapon.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, const int32 /*CurAmmo*/, const int32 /*MaxAmmo*/);
/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API AGunWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
    AGunWeapon();

    FOnAmmoChanged OnAmmoChanged;
    void OnAmmoUpdated(FPrimaryAssetId ChangedAmmoID, int32 NewQuantity);

protected:
    virtual void BeginPlay() override;
    virtual void OnEquip() override;
    virtual void OnUnequip() override;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Ammo")
    int32 CurAmmo;

    UPROPERTY()
    TObjectPtr<class UInventoryComponent> Inventory; // 장전 시 인벤토리를 조회하여 총알 아이템을 찾는다

public:
    virtual void StartAttack() override;
    virtual bool CanAttack() const override { return bCanFire; }

    // 총기 전용 기능들
    void Fire();
    void Reload();
    bool CanReload();

    FTimerHandle FireTimerHandle;

    void OnReloadMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
    FORCEINLINE bool HasAmmo() const { return CurAmmo >= 0 ? true : false; }
    
    // 총 발사 시 카메라 발동시킬 카메라 셰이크 타입
    ECameraShakeType CameraShakeType;

private:
    // 총기 데이터로 캐스팅해서 사용
    const class UGunData* GetGunData() const { return Cast<UGunData>(WeaponData); }
    const class UAmmoItemData* AmmoData;

    bool bCanFire;
    void SetCanFire() { bCanFire = true; }
    int32 GetTotalAmmo();
    void ShakeCamera();
};
