// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData.h"
#include "AmmoItemData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UAmmoItemData : public UItemData
{
	GENERATED_BODY()
public:
	UAmmoItemData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}
};
