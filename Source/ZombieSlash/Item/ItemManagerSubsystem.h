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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void PreloadItemData();

	// 특정 아이템 데이터 비동기 로드
	void LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded);
	// 블루프린트에서 호출할 래퍼 함수
	UFUNCTION(BlueprintCallable)
	void LoadItemDataAsync_BP(FPrimaryAssetId ItemID, const FOnItemDataLoaded& OnLoaded);

	// 무기 데이터 비동기 로드
	//void LoadWeaponDataAsync(FPrimaryAssetId WeaponID, TFunction<void(class UWeaponItemData*)> OnLoaded);

	// 캐시에서 아이템 데이터 가져오기
	UItemData* GetCachedItemData(FPrimaryAssetId ItemID);

private:
	// 아이템 데이터 캐시
	TMap<FPrimaryAssetId, UItemData*> ItemDataCache;

	// LRU 캐시 크기 제한
	static constexpr int32 MaxCacheSize = 100;

	// 캐시 관리
	void AddToCache(FPrimaryAssetId ItemID, UItemData* ItemData);
	void EvictCache();
};
