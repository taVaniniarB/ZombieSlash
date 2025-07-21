// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "CharacterNPC.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API ACharacterNPC : public ACharacterBase
{
	GENERATED_BODY()

public:
	ACharacterNPC();

protected:
	virtual void SetDead() override;

	// �Ӹ� ���� �ߴ� UI ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HPBar;

};
