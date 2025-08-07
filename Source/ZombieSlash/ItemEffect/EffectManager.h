// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UEffectManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEffectManager();

	// ���� ���� ���� ����Ʈ �迭
	UPROPERTY()
	TArray<class UItemEffect*> ActiveEffects;

	UFUNCTION(BlueprintCallable)
	const TArray<UItemEffect*>& GetActiveEffects() const { return ActiveEffects; }

	// HUD ǥ�� ��
	// EffectBarWidget ����� CharacterReference->EffectManager->GetActiveEffects()�� �����ͼ� For Each�� ����Ʈ ������ �а� ���� EffectIconWidget�� �����Ͽ� ǥ��
	// ���� ���� ���� ���� OnEffectChanged ������� UI�� �����ϰų�, 0.2�� �ֱ��� SetTimer Ȱ��

	void AddEffect(UItemEffect* NewEffect);
	void ClearEffect(UItemEffect* Effect);
	void ClearAllEffects();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
