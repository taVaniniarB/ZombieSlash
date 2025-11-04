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
	// Subsystem이 언리얼 엔진 종료 시점에 해제될 때 메모리 누수가 발생하는 문제를 해결하기 위함

	CacheMap.Empty(); // 노트 포인터 보유하기 때문에 먼저 비워야 함 (Dangling Pointer 방지)
	CacheList.Empty();
	CachedItems.Empty();

	Super::Deinitialize();
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
		UE_LOG(LogTemp, Warning, TEXT("Cannot Found Metadata: %s"), *InID.ToString());
		return false;
	}
}

// 캐시에서 찾아보고 없으면 비동기 로드
void UItemManagerSubsystem::LoadItemDataAsync(FPrimaryAssetId ItemID, TFunction<void(class UItemData*)> OnLoaded)
{
	// 캐시 히트
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
	auto NodePtr = CacheMap.Find(ItemID);

	// 캐시 히트
	if (NodePtr)
	{
		// LRU 갱신
		auto Node = *NodePtr;
		MoveToHead(Node);
		return Node->GetValue().Value.Get();
	}

	return nullptr;
}

void UItemManagerSubsystem::AddToCache(FPrimaryAssetId ItemID, UItemData* ItemData)
{
	// 이미 캐시에 존재하면 LRU 갱신 후 종료
	if (CacheMap.Contains(ItemID))
	{
		auto NodePtr = CacheMap.Find(ItemID);
		auto Node = *NodePtr;
		// LRU 갱신
		MoveToHead(Node);
		return;
	}

	// 새 항목 추가
	FLRUCacheEntry NewEntry{ ItemID, ItemData };
	CacheList.AddHead(NewEntry);
	CacheMap.Add(ItemID, CacheList.GetHead());

	// 같은 Id 아이템이 여러개 들어가는 것을 방지
	CachedItems.AddUnique(ItemData);

	// 캐시 크기 초과 시 제거
	if (CacheList.Num() > MaxCacheSize)
	{
		EvictCache();
	}
}

void UItemManagerSubsystem::EvictCache()
{
	// LRU 기반으로 오래된 항목 제거
	auto TailNode = CacheList.GetTail();
	if (TailNode)
	{
		CachedItems.Remove(TailNode->GetValue().Value);
		CacheMap.Remove(TailNode->GetValue().Key);
		CacheList.RemoveNode(TailNode);
	}
}

void UItemManagerSubsystem::MoveToHead(TDoubleLinkedList<FLRUCacheEntry>::TDoubleLinkedListNode* Node)
{
	CacheList.RemoveNode(Node, false);
	CacheList.AddHead(Node);
}