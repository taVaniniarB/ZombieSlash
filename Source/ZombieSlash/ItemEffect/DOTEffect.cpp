// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/DOTEffect.h"
#include "Engine/DamageEvents.h"

void UDOTEffect::Tick(float DT)
{
	Super::Tick(DT);

	TimeSinceLastTick += DT;
	if (TimeSinceLastTick >= TickInterval)
	{
		TimeSinceLastTick = 0.0f;

		if (AActor* DamageTarget = Target.Get())
		{
			FDamageEvent DamageEvent;
			DamageTarget->TakeDamage(DamagePerTick, DamageEvent, nullptr, nullptr);
		}
	}
}
