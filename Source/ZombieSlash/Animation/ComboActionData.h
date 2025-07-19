// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix; // 몽타주 섹션 이름

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount; // 총 몇 개의 콤보가 있는지

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate; // 프레임의 기준 재생 속도 (정확한 타이밍 체크 위해)
	// 그냥 애니메이션 클립에서 보이는 프레임 숫자를 쓰기편하게 초로 환산해주는 값

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount; // 입력이 사전에 입력됐는지 감지하는 프레임들을 저장
};
