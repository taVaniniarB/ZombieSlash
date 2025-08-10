// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemManagerSubsystem.h"
#include "Engine/AssetManager.h"
#include "ItemData.h"
#include "GameData/ItemMetaData.h"

UItemManagerSubsystem::UItemManagerSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemMetadatatRef(TEXT("/Script/Engine.DataTable'/Game/GameData/ItemMetaDataTable.ItemMetaDataTable'"));
	if (nullptr != ItemMetadatatRef.Object)
	{
		ItemMetadataTable = ItemMetadatatRef.Object;
	}
}

void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("ItemData"), AssetIDs); //AssetIDs에 ItemData로 시작하는 아이디 가진 에셋들 저장

	// AssetIDs 순회하며 로드
	for (const FPrimaryAssetId& ID : AssetIDs)
	{
		AssetManager.LoadPrimaryAsset(ID, TArray<FName>(), FStreamableDelegate(), 0);
	}
}

bool UItemManagerSubsystem::GetMetadata(FPrimaryAssetId InID, FItemMetadata& OutMetadata) const
{
	if (!ItemMetadataTable) return false;

	// Row Name은 PrimaryAssetId의 Name 부분
	FName RowName = InID.PrimaryAssetName;
	FItemMetadata* Metadata = ItemMetadataTable->FindRow<FItemMetadata>(RowName, TEXT("ItemMetadata Lookup"));

	if (Metadata)
	{
		OutMetadata = *Metadata;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("메타데이터 찾지 못함: %s"), *InID.ToString());
		return false;
	}
}

// 캐시에서 찾아보고 없으면 비동기 로드
void UItemManagerSubsystem::LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded)
{
	// 캐시에 아이템이 있다
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
				// ID로 아이템 데이터를 찾기
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
	// LRU 기반으로 오래된 항목 제거
	TArray<FPrimaryAssetId> Keys;
	ItemDataCache.GetKeys(Keys);
	if (Keys.Num() > 0)
	{
		ItemDataCache.Remove(Keys[0]);
	}
}