// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "GunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API AGunWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
    AGunWeapon();

protected:
    virtual void BeginPlay() override;

public:
    virtual void OnEquip() override;
    virtual void OnUnequip() override;


protected:
    /*UPROPERTY(BlueprintReadOnly)
    float CurDelay;*/

    UPROPERTY(BlueprintReadOnly, Category = "Ammo")
    int32 CurAmmo;

    UPROPERTY()
    TObjectPtr<class UInventoryComponent> Inventory;

public:
    virtual void StartAttack() override;
    //virtual bool CanAttack() const override;

    // �ѱ� ���� ��ɵ�
    void Fire();
    void Reload();
    bool CanReload();

    FTimerHandle FireTimerHandle;

    void OnReloadMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
    FORCEINLINE bool HasAmmo() const { return CurAmmo >= 0 ? true : false; }

private:
    // �ѱ� �����ͷ� ĳ�����ؼ� ���
    const class UGunData* GetGunData() const { return Cast<UGunData>(WeaponData); }

    bool bCanFire;
    void SetCanFire() { bCanFire = true; }
};
