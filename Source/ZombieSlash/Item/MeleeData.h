// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/WeaponData.h"
#include "MeleeData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UMeleeData : public UWeaponData
{
	GENERATED_BODY()
	
public:
	UMeleeData();

public:
	//float AttackRadius;
	//float AttackRange; // Modifier 스탯에 저장된 값 사용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UComboActionData> ComboActionData;
};
