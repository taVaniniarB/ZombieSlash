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
    TObjectPtr<class UInventoryComponent> Inventory; // ���� �� �κ��丮�� ��ȸ�Ͽ� �Ѿ� �������� ã�´�

public:
    virtual void StartAttack() override;
    virtual bool CanAttack() const override { return bCanFire; }

    // �ѱ� ���� ��ɵ�
    void Fire();
    void Reload();
    bool CanReload();

    FTimerHandle FireTimerHandle;

    void OnReloadMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
    FORCEINLINE bool HasAmmo() const { return CurAmmo >= 0 ? true : false; }
    
    // �� �߻� �� ī�޶� �ߵ���ų ī�޶� ����ũ Ÿ��
    ECameraShakeType CameraShakeType;

private:
    // �ѱ� �����ͷ� ĳ�����ؼ� ���
    const class UGunData* GetGunData() const { return Cast<UGunData>(WeaponData); }
    const class UAmmoItemData* AmmoData;

    bool bCanFire;
    void SetCanFire() { bCanFire = true; }
    int32 GetTotalAmmo();
    void ShakeCamera();
};
