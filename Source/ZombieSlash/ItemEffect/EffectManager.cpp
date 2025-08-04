// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/EffectManager.h"
#include "EffectManager.h"
#include "ItemEffect.h"

UEffectManager::UEffectManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEffectManager::AddEffect(UItemEffect* NewEffect)
{
	for (UItemEffect* Effect : ActiveEffects)
	{
		// 중복 -> 오래된 것 제거
		if (Effect && Effect->IsSameType(NewEffect))
		{
			Effect->Remove();
			ActiveEffects.Remove(Effect);
			break;
		}
	}

	NewEffect->Apply(GetOwner());
	ActiveEffects.Add(NewEffect);
}

void UEffectManager::RemoveEffect(UItemEffect* Effect)
{
	if (Effect)
	{
		Effect->Remove();
		ActiveEffects.Remove(Effect);
	}
}

void UEffectManager::ClearAllEffects()
{
	for (UItemEffect* Effect : ActiveEffects)
	{
		if (Effect)
		{
			Effect->Remove();
		}
	}
	ActiveEffects.Empty();
}

void UEffectManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		if (UItemEffect* Effect = ActiveEffects[i])
		{
			Effect->Tick(DeltaTime);
		}
	}
}

