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
	UPROPERTY()
	float MaxRange;

	UPROPERTY()
	int32 MaxAmmo;

	UPROPERTY()
	float Delay;
};
