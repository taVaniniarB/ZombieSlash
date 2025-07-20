// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/CharacterStat.h"
#include "CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float /*CurHP*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

	FOnHPZeroDelegate OnHPZero;
	FOnHPChangedDelegate OnHPChanged;

	void SetStat(FName ID);

	float ApplyDamage(float InDamage);


	FORCEINLINE const FCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FCharacterStat& GetModifierStat() const { return ModifierStat; }
	// 이 임시 객체에 대한 레퍼런스를 반환하게 되면, 함수가 종료되는 순간 임시 객체는 소멸되고,
	// 반환된 레퍼런스는 유효하지 않은 메모리(dangling reference)를 가리키게 되어 예측 불가능한 버그나 크래시를 유발할 수 있습니다.
	FORCEINLINE FCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	FORCEINLINE float GetMaxHP() { return GetTotalStat().MaxHP; }
	FORCEINLINE float GetCurHP() { return CurHP; }

	FORCEINLINE void HealHp(float InHealAmount) { /*CurHP = FMath::Clamp(CurHP + InHealAmount, 0, GetTotalStat().MaxHp); OnHPChanged.Broadcast(CurHP);*/ }


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:	
	void SetHP(float NewHP);
	
protected:
	//HP 멤버변수는 각 인스턴스마다 다를 수 있기 때문에(예: 플레이어와 보스몹의 HP는 다르다)
	// UPROPERTY 지정자로 VisibleInstanceOnly를 붙여준다. BP클래스 에디터에서 클래스 기본값을 보여주는 것이 무의미함.
	//UPROPERTY(VisibleInstanceOnly, Category = Stat)
	//float MaxHP;

	// 현재 HP 값은 게임 할 때마다 새롭게 저장되기에 디스크에 저장할 필요가 없다. Transient 붙임으로써 디스크 저장 시 불필요한 공간을 낭비하지 않도록 한다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FCharacterStat ModifierStat;
};
