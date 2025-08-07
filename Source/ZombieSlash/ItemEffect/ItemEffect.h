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
	TWeakObjectPtr<AActor> Target;

	virtual void Apply(AActor* InTarget) { Target = InTarget; }

	virtual void Tick(float DeltaTime);

	virtual void OnExpired();

	virtual void ClearEffect() {}

	virtual bool IsExpired() const { return ElapsedTime >= Duration; }

	// 중복 이펙트 체크용
	virtual bool IsSameType(const UItemEffect* Other) const { return this->GetClass() == Other->GetClass(); }

	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const { return FMath::Max(Duration - ElapsedTime, 0.0f); }
};
