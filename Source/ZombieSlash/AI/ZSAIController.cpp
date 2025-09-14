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

	// 블랙보드 기동
	// BBAsset(블랙보드데이터) 기반으로 실제 블랙보드 메모리 공간 생성
	// GameDev에서는 Run Behavior Tree만 호출해줬는데, 사실 Run Behavior Tree에서 내부적으로 Use Blackboard가 호출됨
	// 유연성(다른 블랙보드 에셋을 지정하는 등 커스텀) 위해 이렇게 한 것 같다 
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
