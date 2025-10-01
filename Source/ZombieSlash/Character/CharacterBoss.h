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

	virtual void OnBossDead();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UWeaponManagerComponent> WeaponManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComp;

	virtual void SetDead() override;
	virtual void AttackByAI() override;

	void UpdateWarpTarget();
};