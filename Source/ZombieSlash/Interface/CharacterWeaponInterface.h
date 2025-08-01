// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESLASH_API ICharacterWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual struct FCharacterStat GetWeaponOwnerStat() const = 0;
	virtual class AController* GetWeaponOwnerController() const = 0;
	virtual class UAnimInstance* GetWeaponOwnerAnimInstance() const = 0;
	virtual FVector GetWeaponOwnerLocation() const = 0;
	virtual FVector GetWeaponOwnerForwardVector() const = 0;
	virtual float GetWeaponOwnerCapsuleRadius() const = 0;
	virtual class UInventoryComponent* GetInventory() const = 0;
	virtual bool GetShouldMove() const = 0;
	virtual void EndReload() = 0;
	virtual void Parry() = 0;
};
