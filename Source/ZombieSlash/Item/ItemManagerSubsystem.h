// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManagerSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemDataLoaded, UItemData*, LoadedData);

UCLASS()
class ZOMBIESLASH_API UItemManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UItemManagerSubsystem();

	static UItemManagerSubsystem* Get(const UObject* WorldContext)
	{
		if (!WorldContext || !WorldContext->GetWorld() || !WorldContext->GetWorld()->GetGameInstance())
			return nullptr;

		return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();
	}
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void PreloadItemData();
	
	// ��Ÿ������ ���̺��� ��Ÿ������ ��������
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool GetMetadata(FPrimaryAssetId InID, struct FItemMetadata& OutMetadata) const;

	// Ư�� ������ ������ �񵿱� �ε�
	void LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded);

	// �������Ʈ���� ȣ���� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void LoadItemDataAsync_BP(FPrimaryAssetId ItemID, const FOnItemDataLoaded& OnLoaded);

	// ĳ�ÿ��� ������ ������ ��������
	UItemData* GetCachedItemData(FPrimaryAssetId ItemID);

private:
	// ������ ��Ÿ������ ���̺�
	UPROPERTY()
	TObjectPtr<UDataTable> ItemMetadataTable;

	// ������ ������ ĳ��
	TMap<FPrimaryAssetId, UItemData*> ItemDataCache;

	// LRU ĳ�� ũ�� ����
	static constexpr int32 MaxCacheSize = 100;

	// ĳ�� ����
	void AddToCache(FPrimaryAssetId ItemID, UItemData* ItemData);
	void EvictCache();
};
