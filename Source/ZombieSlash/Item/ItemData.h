#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/ItemType.h"
#include "ItemData.generated.h"


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
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack;
};
