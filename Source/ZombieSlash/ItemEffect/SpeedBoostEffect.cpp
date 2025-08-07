// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect/SpeedBoostEffect.h"
#include "SpeedBoostEffect.h"
#include "Interface/CharacterStatInterface.h"

void USpeedBoostEffect::Apply(AActor* InTarget)
{
    Super::Apply(InTarget);

    if (ICharacterStatInterface* Character = Cast<ICharacterStatInterface>(InTarget))
    {
        Character->ApplySpeedBuff(SpeedMultiplier);
    }
}

void USpeedBoostEffect::ClearEffect()
{
    Super::ClearEffect();
    UE_LOG(LogTemp, Warning, TEXT("Speed Boost Effect Remove Called"));
    if (ICharacterStatInterface* Character = Cast<ICharacterStatInterface>(Target))
    {
        Character->ApplySpeedBuff(1.0f);
    }
}
