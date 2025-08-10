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

	// ĳ���� ���� ���̺��� ���������� �����ؼ� �ʿ��� ���� ��ü�鿡�� �����ϵ��� �Ѵ�.
	FCharacterStat GetCharacterStat(FName InID) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> CharacterStatTable;
};
