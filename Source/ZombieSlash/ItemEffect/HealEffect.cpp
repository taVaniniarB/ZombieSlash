// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/HealEffect.h"
#include "Interface/CharacterStatInterface.h"

void UHealEffect::Apply(AActor* InTarget)
{
	if (ICharacterStatInterface* Character = Cast<ICharacterStatInterface>(InTarget))
	{
		Character->ApplyHeal(HealAmount);
	}
}
