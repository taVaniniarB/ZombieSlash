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

	// Ư�� ������ ������ �񵿱� �ε�
	void LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded);
	// �������Ʈ���� ȣ���� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void LoadItemDataAsync_BP(FPrimaryAssetId ItemID, const FOnItemDataLoaded& OnLoaded);

	// ���� ������ �񵿱� �ε�
	//void LoadWeaponDataAsync(FPrimaryAssetId WeaponID, TFunction<void(class UWeaponItemData*)> OnLoaded);

	// ĳ�ÿ��� ������ ������ ��������
	UItemData* GetCachedItemData(FPrimaryAssetId ItemID);

private:
	// ������ ������ ĳ��
	TMap<FPrimaryAssetId, UItemData*> ItemDataCache;

	// LRU ĳ�� ũ�� ����
	static constexpr int32 MaxCacheSize = 100;

	// ĳ�� ����
	void AddToCache(FPrimaryAssetId ItemID, UItemData* ItemData);
	void EvictCache();
};
