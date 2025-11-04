// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManagerSubsystem.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemDataLoaded, UItemData*, LoadedData);


// LRU 캐시 항목 구조체
USTRUCT()
struct FLRUCacheEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FPrimaryAssetId Key;

	UPROPERTY()
	TObjectPtr<UItemData> Value;

	FLRUCacheEntry()
		: Key(), Value(nullptr)
	{
	}
	FLRUCacheEntry(FPrimaryAssetId InKey, UItemData* InValue)
		: Key(InKey), Value(InValue)
	{
	}
};

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

	// 메타데이터 테이블에서 메타데이터 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool GetMetadata(FPrimaryAssetId InID, struct FItemMetadata& OutMetadata) const;

	// 특정 아이템 데이터 비동기 로드
	void LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded);

	// 블루프린트에서 호출할 래퍼 함수
	UFUNCTION(BlueprintCallable)
	void LoadItemDataAsync_BP(FPrimaryAssetId ItemID, const FOnItemDataLoaded& OnLoaded);

	// 캐시에서 아이템 데이터 가져오기
	UItemData* GetCachedItemData(FPrimaryAssetId ItemID);

private:
	// 아이템 메타데이터 테이블
	UPROPERTY()
	TObjectPtr<UDataTable> ItemMetadataTable;

private:
	// 가비지 컬렉션으로부터 보호하기 위해 캐시된 아이템 데이터를 보관
	UPROPERTY()
	TArray<TObjectPtr<UItemData>> CachedItems;

	// 캐시 리스트 - LRU(Least Recently Used) 캐시 교체 정책을 구현하기 위해 사용
	TDoubleLinkedList<FLRUCacheEntry> CacheList;
	// 캐시 맵 - FPrimaryAssetId를 키로 사용하여 캐시된 아이템 데이터를 빠르게 찾기 위해 사용
	TMap<FPrimaryAssetId, TDoubleLinkedList<FLRUCacheEntry>::TDoubleLinkedListNode*> CacheMap;

	// LRU 캐시 크기 제한
	UPROPERTY(EditDefaultsOnly) 
	int32 MaxCacheSize = 100;
	
	// 캐시 관리
	void AddToCache(FPrimaryAssetId ItemID, UItemData* ItemData);
	void EvictCache();

	void MoveToHead(TDoubleLinkedList<FLRUCacheEntry>::TDoubleLinkedListNode* Node);
};
