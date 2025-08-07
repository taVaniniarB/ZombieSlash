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

	// 현재 적용 중인 이펙트 배열
	UPROPERTY()
	TArray<class UItemEffect*> ActiveEffects;

	UFUNCTION(BlueprintCallable)
	const TArray<UItemEffect*>& GetActiveEffects() const { return ActiveEffects; }

	// HUD 표기 시
	// EffectBarWidget 만들고 CharacterReference->EffectManager->GetActiveEffects()로 가져와서 For Each로 이펙트 정보를 읽고 개별 EffectIconWidget을 생성하여 표시
	// 성능 문제 방지 위해 OnEffectChanged 방식으로 UI를 갱신하거나, 0.2초 주기의 SetTimer 활용

	void AddEffect(UItemEffect* NewEffect);
	void ClearEffect(UItemEffect* Effect);
	void ClearAllEffects();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
