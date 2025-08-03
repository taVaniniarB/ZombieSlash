// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatComponent.h"
#include "CharacterStatComponent.h"
#include "Interface/CharacterStatInterface.h"
#include "GameData/ZSGameSingleton.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	bWantsInitializeComponent = true;
}

void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FName OwnerID = Cast<ICharacterStatInterface>(GetOwner())->GetCharacterID();
	if (OwnerID.GetStringLength() <= 0)
		UE_LOG(LogTemp, Warning, TEXT("오너 아이디 찾지 못함"));
	SetBaseStat(OwnerID);
	SetHP(GetMaxHP());
}

void UCharacterStatComponent::SetBaseStat(FName InID)
{
	BaseStat = UZSGameSingleton::Get().GetCharacterStat(InID);
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	//check(BaseStat.MaxHP > 0);
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

void UCharacterStatComponent::ApplyHeal(float InHealAmount)
{
	SetHP(CurHP + InHealAmount);
}

void UCharacterStatComponent::SetHP(float NewHP)
{
	CurHP = FMath::Clamp<float>(NewHP, 0.f, GetMaxHP());
	
	OnHPChanged.Broadcast(CurHP);
}
