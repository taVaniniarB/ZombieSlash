// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData.h"
#include "HealItemData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UHealItemData : public UItemData
{
	GENERATED_BODY()
	
public:
	UHealItemData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}

	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;
};
