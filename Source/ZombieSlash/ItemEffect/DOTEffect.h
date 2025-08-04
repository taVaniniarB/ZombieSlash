// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect/ItemEffect.h"
#include "DOTEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UDOTEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamagePerTick = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TickInterval = 1.0f;

	float TimeSinceLastTick = 0.0f;
	
	virtual void Tick(float DT) override;
};
