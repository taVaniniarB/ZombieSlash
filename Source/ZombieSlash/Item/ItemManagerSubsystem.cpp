// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemManagerSubsystem.h"
#include "Engine/AssetManager.h"
#include "ItemData.h"
#include "GameData/ItemMetaData.h"
#include "GameData/ZSGameSingleton.h"

void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	PreloadItemData();
}

void UItemManagerSubsystem::Deinitialize()
{
	ItemDataCache.Empty();
	Super::Deinitialize();
}

void UItemManagerSubsystem::PreloadItemData()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> AssetIDs;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("ItemData"), AssetIDs); //AssetIDs�� ItemData�� �����ϴ� ���̵� ���� ���µ� ����

	// AssetIDs ��ȸ�ϸ� �ε�
	for (const FPrimaryAssetId& ID : AssetIDs)
	{
		AssetManager.LoadPrimaryAsset(ID, TArray<FName>(), FStreamableDelegate(), 0);
	}
}

// ĳ�ÿ��� ã�ƺ��� ������ �񵿱� �ε�
void UItemManagerSubsystem::LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded)
{
	// ĳ�ÿ� �������� �ִ�
	if (UItemData* CachedData = GetCachedItemData(ItemID))
	{
		OnLoaded(CachedData);
		return;
	}

	UAssetManager& AssetManager = UAssetManager::Get();
	AssetManager.LoadPrimaryAsset(ItemID, TArray<FName>(),
		FStreamableDelegate::CreateLambda(
			[this, ItemID, OnLoaded]()
			{
				// ID�� ������ �����͸� ã��
				UItemData* ItemData = Cast<UItemData>(UAssetManager::Get().GetPrimaryAssetObject(ItemID));
				if (ItemData)
				{
					AddToCache(ItemID, ItemData);
					OnLoaded(ItemData);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to load ItemData: %s"), *ItemID.ToString());
					OnLoaded(nullptr);
				}
			}), 0);
}

void UItemManagerSubsystem::LoadItemDataAsync_BP(FPrimaryAssetId ItemID, const FOnItemDataLoaded& OnLoaded)
{
	LoadItemDataAsync(ItemID, [OnLoaded](UItemData* ItemData)
		{
			AsyncTask(ENamedThreads::GameThread, [OnLoaded, ItemData]()
				{
					OnLoaded.ExecuteIfBound(ItemData);
				});
		});

}

UItemData* UItemManagerSubsystem::GetCachedItemData(FPrimaryAssetId ItemID)
{
	UItemData** CachedItem = ItemDataCache.Find(ItemID);
	return CachedItem ? *CachedItem : nullptr;
}

bool UItemManagerSubsystem::GetMetadata(const FPrimaryAssetId& ItemID, FItemMetadata& OutMetadata) const
{
	return UZSGameSingleton::Get().GetItemMetadata(ItemID, OutMetadata);
}

void UItemManagerSubsystem::AddToCache(FPrimaryAssetId ItemID, UItemData* ItemData)
{
	if (ItemDataCache.Num() >= MaxCacheSize)
	{
		EvictCache();
	}
	ItemDataCache.Add(ItemID, ItemData);
}

void UItemManagerSubsystem::EvictCache()
{
	// LRU ������� ������ �׸� ����
	TArray<FPrimaryAssetId> Keys;
	ItemDataCache.GetKeys(Keys);
	if (Keys.Num() > 0)
	{
		ItemDataCache.Remove(Keys[0]);
	}
}