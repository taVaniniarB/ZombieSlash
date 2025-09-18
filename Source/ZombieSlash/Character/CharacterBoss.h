// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterNPC.h"
#include "CharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API ACharacterBoss : public ACharacterNPC
{
	GENERATED_BODY()
	
public:
	ACharacterBoss();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UWeaponManagerComponent> WeaponManager;
};