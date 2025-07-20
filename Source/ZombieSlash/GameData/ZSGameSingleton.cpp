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
