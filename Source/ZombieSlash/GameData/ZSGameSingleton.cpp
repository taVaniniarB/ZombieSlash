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