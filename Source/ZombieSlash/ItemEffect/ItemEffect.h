// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class ZOMBIESLASH_API UItemEffect : public UObject
{
	GENERATED_BODY()
	
public:
	// 효과 적용 함수
	UFUNCTION(BlueprintCallable, Category = "Item Effect")
	virtual void ApplyEffect(AActor* Target) PURE_VIRTUAL(UItemEffect::ApplyEffect, );

	UFUNCTION(BlueprintCallable, Category = "Item Effect")
	virtual void RemoveEffect(AActor* Target) {};
};
