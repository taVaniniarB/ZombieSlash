// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect/ItemEffect.h"
#include "HealEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UHealEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float HealAmount = 20.0f;

	virtual void Apply(AActor* Target) override;
};
