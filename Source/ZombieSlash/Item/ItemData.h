// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	HealItem,
	Ammo
};

/**
 * 
 */
UCLASS(BlueprintType)
class ZOMBIESLASH_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack;
};
