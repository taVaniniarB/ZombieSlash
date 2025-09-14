// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESLASH_API ICharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRange() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
};
