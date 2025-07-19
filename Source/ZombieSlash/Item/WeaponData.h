// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData.h"
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

	TObjectPtr<UStaticMeshComponent> WeaponMesh;
};
