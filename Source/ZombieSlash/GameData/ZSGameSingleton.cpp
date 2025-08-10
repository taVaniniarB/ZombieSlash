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
	// ������ �̱��� Ŭ���� �����ϸ� �ν��Ͻ��� ����鼭 GEngine�� ��������� �Ҵ���� ����

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
			UE_LOG(LogTemp, Warning, TEXT("���� ã�� ����"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatTable was null"));

	return FCharacterStat();
}