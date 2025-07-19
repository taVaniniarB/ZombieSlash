// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	float ApplyDamage(float InDamage);


	/*FORCEINLINE const FCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }*/

	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurHP() { return CurHP; }

	FORCEINLINE void HealHp(float InHealAmount) { /*CurHP = FMath::Clamp(CurHP + InHealAmount, 0, GetTotalStat().MaxHp); OnHPChanged.Broadcast(CurHP);*/ }


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:	
	void SetHP(float NewHP);
	
protected:
	//HP ��������� �� �ν��Ͻ����� �ٸ� �� �ֱ� ������(��: �÷��̾�� �������� HP�� �ٸ���)
	// UPROPERTY �����ڷ� VisibleInstanceOnly�� �ٿ��ش�. BPŬ���� �����Ϳ��� Ŭ���� �⺻���� �����ִ� ���� ���ǹ���.
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHP;

	// ���� HP ���� ���� �� ������ ���Ӱ� ����Ǳ⿡ ��ũ�� ������ �ʿ䰡 ����. Transient �������ν� ��ũ ���� �� ���ʿ��� ������ �������� �ʵ��� �Ѵ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurHP;
};
