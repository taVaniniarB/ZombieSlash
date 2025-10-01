// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZSAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API AZSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AZSAIController();

	void RunAI();
	void StopAI();

protected:
	// � ��Ʈ�ѷ��� ���� ������ �� ȣ��Ǵ� �̺�Ʈ �Լ�
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	TObjectPtr<class UBehaviorTree> BTAsset;
};
