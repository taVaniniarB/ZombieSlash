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
	//float AttackRange; // Modifier 스탯에 저장된 값 사용

	// 전신에 적용된다. 정지 상태에서의 콤보 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> InPlaceAttackMontage;

	// Upper Body에만 적용된다. 이동 중 공격하면 재생될 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> MoveAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UComboActionData> ComboActionData;
};
