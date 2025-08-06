#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/ItemData.h"
#include "ItemMetadata.generated.h"


USTRUCT(BlueprintType)
struct FItemMetadata : public FTableRowBase
{
	GENERATED_BODY()

public:
	// FPrimaryAssetId는 직렬화 불가하므로 FName으로 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName PrimaryAssetId;

	// 표시용 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType = EItemType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};