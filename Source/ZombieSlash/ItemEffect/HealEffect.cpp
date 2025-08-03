// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/HealEffect.h"
#include "Interface/CharacterStatInterface.h"

void UHealEffect::ApplyEffect(AActor* Target)
{
	if (ICharacterStatInterface* Character = Cast<ICharacterStatInterface>(Target))
	{
		Character->ApplyHeal(HealAmount);
	}
}
