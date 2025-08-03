// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData.h"
#include "UsableItemData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UUsableItemData : public UItemData
{
	GENERATED_BODY()

public:
	UUsableItemData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}

	// 아이템 효과 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TArray<TSubclassOf<class UItemEffect>> Effects;
};
