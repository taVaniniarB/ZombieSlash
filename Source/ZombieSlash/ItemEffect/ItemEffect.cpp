// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/ItemEffect.h"
#include "EffectManager.h"

void UItemEffect::Tick(float DeltaTime)
{
    ElapsedTime += DeltaTime;
    if (ElapsedTime >= Duration)
    {
        OnExpired();
    }
}

void UItemEffect::OnExpired()
{
    if (Target.IsValid())
    {
        if (UEffectManager* Manager = Target->FindComponentByClass<UEffectManager>())
        {
            Manager->ClearEffect(this);
        }
    }
}
