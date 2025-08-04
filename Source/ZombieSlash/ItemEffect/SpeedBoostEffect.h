// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect/ItemEffect.h"
#include "GameData/CharacterStat.h"
#include "SpeedBoostEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API USpeedBoostEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SpeedMultiplier = 1.5f;

	virtual void Apply(AActor* Target) override;
	virtual void Remove() override;

	UPROPERTY(EditAnywhere, Category = Stat)
	FCharacterStat ItemEffectStat;

private:
	FTimerHandle TimerHandle;
};
