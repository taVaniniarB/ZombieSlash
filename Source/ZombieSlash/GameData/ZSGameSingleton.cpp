// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ZSGameSingleton.h"
#include "ZSGameSingleton.h"

UZSGameSingleton::UZSGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatRef(TEXT("/Script/Engine.DataTable'/Game/GameData/CharacterStatTable.CharacterStatTable'"));
	if (nullptr != CharacterStatRef.Object)
	{
		CharacterStatTable = CharacterStatRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemMetadatatRef(TEXT("/Script/Engine.DataTable'/Game/GameData/ItemMetaDataTable.ItemMetaDataTable'"));
	if (nullptr != ItemMetadatatRef.Object)
	{
		ItemMetadataTable = ItemMetadatatRef.Object;
	}
}

UZSGameSingleton& UZSGameSingleton::Get()
{
	UZSGameSingleton* Singleton = CastChecked<UZSGameSingleton>(GEngine->GameSingleton);
	// 엔진에 싱글톤 클래스 설정하면 인스턴스를 만들면서 GEngine의 멤버변수로 할당까지 해줌

	if (Singleton)
	{
		return *Singleton;
	}
	return *NewObject<UZSGameSingleton>();
}

FCharacterStat UZSGameSingleton::GetCharacterStat(FName InID) const
{
	if (CharacterStatTable)
	{
		FCharacterStat* Stat = CharacterStatTable->FindRow<FCharacterStat>(InID, TEXT("GetCharacterStat By ID"));
		if (Stat)
		{
			return *Stat;
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("스탯 찾지 못함"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatTable was null"));

	return FCharacterStat();
}

bool UZSGameSingleton::GetItemMetadata(FPrimaryAssetId InID, FItemMetadata& OutMetadata) const
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