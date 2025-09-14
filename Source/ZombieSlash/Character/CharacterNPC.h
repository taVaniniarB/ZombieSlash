// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interface/CharacterAIInterface.h"
#include "CharacterNPC.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API ACharacterNPC : public ACharacterBase, public ICharacterAIInterface
{
	GENERATED_BODY()

public:
	ACharacterNPC();

protected:
	virtual void SetDead() override;
	virtual void Attack() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> AttackMontage;

	// AI Interface
	virtual float GetAIPatrolRange() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void AttackByAI() override;
	virtual void SetAttackFinishedDelegate(FAICharacterAttackFinished InOnAttackFinished) override;

	FAICharacterAttackFinished OnAttackFinished;

	// ¸Ó¸® À§¿¡ ¶ß´Â UI À§Á¬
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HPBar;

};
