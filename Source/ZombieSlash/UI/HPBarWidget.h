// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZSUserWidget.h"
#include "GameData/CharacterStat.h"
#include "HPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UHPBarWidget : public UZSUserWidget
{
	GENERATED_BODY()
	
public:
	UHPBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMaxHP(float InMaxHP) { MaxHP = InMaxHP; }

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FCharacterStat& BaseStat, const FCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurHP);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	UPROPERTY()
	float CurHP;
	UPROPERTY()
	float MaxHP;
};
