// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterStatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESLASH_API ICharacterStatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FName GetCharacterID() const = 0;
	virtual struct FCharacterStat GetTotalStat() const = 0;
	virtual void ApplyItemEffectStat(struct FCharacterStat InItemEffectStat) = 0;
	virtual void ResetItemEffectStat() = 0;
	virtual void ApplyHeal(float InHealAmount) = 0;
	virtual void ApplySpeedBuff(float InMultiflier) = 0;
};
