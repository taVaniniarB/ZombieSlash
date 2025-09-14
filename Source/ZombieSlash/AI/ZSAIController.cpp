// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ZSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZSAI.h"

AZSAIController::AZSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprint/ZombieAI/BB_Zombie.BB_Zombie'"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/ZombieAI/BT_Zombie.BT_Zombie'"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AZSAIController::RunAI()
{
	UBlackboardComponent* BBComp = Blackboard.Get();

	// ������ �⵿
	// BBAsset(�����嵥����) ������� ���� ������ �޸� ���� ����
	// GameDev������ Run Behavior Tree�� ȣ������µ�, ��� Run Behavior Tree���� ���������� Use Blackboard�� ȣ���
	// ������(�ٸ� ������ ������ �����ϴ� �� Ŀ����) ���� �̷��� �� �� ���� 
	if (UseBlackboard(BBAsset, BBComp))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AZSAIController::StopAI()
{
}

void AZSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
