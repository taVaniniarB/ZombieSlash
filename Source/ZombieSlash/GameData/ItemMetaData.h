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
	// FPrimaryAssetId�� ����ȭ �Ұ��ϹǷ� FName���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName PrimaryAssetId;

	// ǥ�ÿ� ������
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