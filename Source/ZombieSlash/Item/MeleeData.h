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
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}

public:
	//float AttackRadius;
	//float AttackRange; // Modifier ���ȿ� ����� �� ���

	// ���ſ� ����ȴ�. ���� ���¿����� �޺� ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> InPlaceAttackMontage;

	// Upper Body���� ����ȴ�. �̵� �� �����ϸ� ����� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> MoveAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UComboActionData> ComboActionData;
};
