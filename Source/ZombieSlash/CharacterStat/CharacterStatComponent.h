// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/CharacterStat.h"
#include "CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float /*CurHP*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FCharacterStat& /*BaseStat*/, const FCharacterStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHPZeroDelegate OnHPZero;
	FOnHPChangedDelegate OnHPChanged;
	FOnStatChangedDelegate OnStatChanged;

	FORCEINLINE void AddBaseStat(const FCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	void SetBaseStat(FName ID);
	FORCEINLINE void SetBaseStat(const FCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetItemEffectStat(const FCharacterStat& InItemEffectStat) { ItemEffectStat = InItemEffectStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void ResetItemEffectStat() { ItemEffectStat = FCharacterStat(); OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }


	float ApplyDamage(float InDamage);
	void ApplyHeal(float InHealAmount);


	FORCEINLINE const FCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE const FCharacterStat& GetItemEffectStat() const { return ItemEffectStat; }
	// �� �ӽ� ��ü�� ���� ���۷����� ��ȯ�ϰ� �Ǹ�, �Լ��� ����Ǵ� ���� �ӽ� ��ü�� �Ҹ�ǰ�,
	// ��ȯ�� ���۷����� ��ȿ���� ���� �޸�(dangling reference)�� ����Ű�� �Ǿ� ���� �Ұ����� ���׳� ũ���ø� ������ �� �ֽ��ϴ�.
	FORCEINLINE FCharacterStat GetTotalStat() const { return BaseStat + ModifierStat + ItemEffectStat; }

	FORCEINLINE float GetMaxHP() { return GetTotalStat().MaxHP; }
	FORCEINLINE float GetCurHP() { return CurHP; }

	FORCEINLINE void HealHp(float InHealAmount) { CurHP = FMath::Clamp(CurHP + InHealAmount, 0, GetTotalStat().MaxHP); OnHPChanged.Broadcast(CurHP); }

	// Buff
	float MovementSpeedMultiplier = 1.0f;
	void ApplySpeedBuff(float InMultiflier) { MovementSpeedMultiplier = InMultiflier; }

protected:	
	void SetHP(float NewHP);
	
protected:
	//HP ��������� �� �ν��Ͻ����� �ٸ� �� �ֱ� ������(��: �÷��̾�� �������� HP�� �ٸ���)
	// UPROPERTY �����ڷ� VisibleInstanceOnly�� �ٿ��ش�. BPŬ���� �����Ϳ��� Ŭ���� �⺻���� �����ִ� ���� ���ǹ���.
	//UPROPERTY(VisibleInstanceOnly, Category = Stat)
	//float MaxHP;

	// ���� HP ���� ���� �� ������ ���Ӱ� ����Ǳ⿡ ��ũ�� ������ �ʿ䰡 ����. Transient �������ν� ��ũ ���� �� ���ʿ��� ������ �������� �ʵ��� �Ѵ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FCharacterStat ModifierStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FCharacterStat ItemEffectStat;
};
