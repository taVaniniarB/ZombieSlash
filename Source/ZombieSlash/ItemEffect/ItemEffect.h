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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Duration = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float ElapsedTime = 0.0f;

	UPROPERTY()
	TWeakObjectPtr<AActor> Target; // 이펙트를 적용할 대상

	virtual void Apply(AActor* InTarget) { Target = InTarget; } // 이펙트를 실제로 적용시키는 함수

	virtual void Tick(float DeltaTime); // 도트 대미지처럼 Tick이 필요한 경우 오버라이드

	virtual void OnExpired(); // 이펙트 Duration 종료 시 호출

	virtual void ClearEffect() {} // Target에 적용된 이펙트를 원상복구하는 함수

	virtual bool IsExpired() const { return ElapsedTime >= Duration; }

	// 중복 이펙트 체크용
	virtual bool IsSameType(const UItemEffect* Other) const { return this->GetClass() == Other->GetClass(); }

	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const { return FMath::Max(Duration - ElapsedTime, 0.0f); }
};
