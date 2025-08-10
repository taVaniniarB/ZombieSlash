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
	TWeakObjectPtr<AActor> Target; // ����Ʈ�� ������ ���

	virtual void Apply(AActor* InTarget) { Target = InTarget; } // ����Ʈ�� ������ �����Ű�� �Լ�

	virtual void Tick(float DeltaTime); // ��Ʈ �����ó�� Tick�� �ʿ��� ��� �������̵�

	virtual void OnExpired(); // ����Ʈ Duration ���� �� ȣ��

	virtual void ClearEffect() {} // Target�� ����� ����Ʈ�� ���󺹱��ϴ� �Լ�

	virtual bool IsExpired() const { return ElapsedTime >= Duration; }

	// �ߺ� ����Ʈ üũ��
	virtual bool IsSameType(const UItemEffect* Other) const { return this->GetClass() == Other->GetClass(); }

	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const { return FMath::Max(Duration - ElapsedTime, 0.0f); }
};
