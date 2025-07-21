// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStat.h"
#include "ZSGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UZSGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UZSGameSingleton();
	static UZSGameSingleton& Get();

	// 캐릭터 스탯 테이블을 내부적으로 보관해서 필요한 게임 객체들에게 제공하도록 한다.
	FCharacterStat GetCharacterStat(FName InID) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> CharacterStatTable;
};
