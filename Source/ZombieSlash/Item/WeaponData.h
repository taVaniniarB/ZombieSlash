// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData.h"
#include "GameData/CharacterStat.h"
#include "Enums/WeaponType.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UWeaponData : public UItemData
{
	GENERATED_BODY()
	
public:
	UWeaponData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = Stat)
	FCharacterStat ModifierStat;

	// WeaponData.h
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AWeaponBase> WeaponActorClass;
};
