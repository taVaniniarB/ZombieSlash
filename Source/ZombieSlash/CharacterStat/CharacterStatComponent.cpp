// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatComponent.h"
#include "CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	SetHP(MaxHP);
}


float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	// 대미지가 음수인 경우 대비
	const float ActualDamage = FMath::Max(0.f, InDamage);
	const float PrevHP = CurHP;

	SetHP(PrevHP - ActualDamage);

	if (CurHP <= KINDA_SMALL_NUMBER)
	{
		OnHPZero.Broadcast();
	}

	return ActualDamage;
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCharacterStatComponent::SetHP(float NewHP)
{
	CurHP = FMath::Clamp<float>(NewHP, 0.f, MaxHP);
}
