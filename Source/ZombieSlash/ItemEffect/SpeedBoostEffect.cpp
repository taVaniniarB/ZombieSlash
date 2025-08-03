// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/SpeedBoostEffect.h"
#include "SpeedBoostEffect.h"
#include "Interface/CharacterStatInterface.h"

void USpeedBoostEffect::ApplyEffect(AActor* Target)
{
    if (ICharacterStatInterface* Character = Cast<ICharacterStatInterface>(Target))
    {
        ItemEffectStat.MovementSpeed = (Character->GetTotalStat().MovementSpeed) * SpeedMultiplier;

        Character->ApplyItemEffectStat(ItemEffectStat);
        
        Target->GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            [this, Target]() { RemoveEffect(Target); },
            Duration,
            false
        );
    }
}

void USpeedBoostEffect::RemoveEffect(AActor* Target)
{
    if (ICharacterStatInterface* Character = Cast<ICharacterStatInterface>(Target))
    {
        Character->ResetItemEffectStat();
    }
}
