// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESLASH_API ICharacterHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupHUDWidget(class UZSHUDWidget* InHUDWidget) = 0;
	
	// OnWeaponEquipped 이벤트를 구독하기 위한 함수
	virtual void BindWeaponEquippedEvent(UZSHUDWidget* InHUDWidget) = 0;
};
