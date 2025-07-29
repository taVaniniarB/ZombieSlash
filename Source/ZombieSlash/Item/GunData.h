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

public:
	UPROPERTY(EditAnywhere)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere)
	float Delay;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ShootMontage;
};
