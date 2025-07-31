// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/WeaponData.h"
#include "GunData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UGunData : public UWeaponData
{
	GENERATED_BODY()
	
public:
	UGunData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = "Ammo")
	TObjectPtr<class UAmmoItemData> AmmoData;
	
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 MaxAmmo; // 탄창에 들어갈 수 있는 최대 총알

	UPROPERTY(EditAnywhere, Category = "Delay")
	float FireDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<class UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<class UNiagaraSystem> MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ShootMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ReloadMontage;
};
