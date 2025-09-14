// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "ZSAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CharacterAIInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ControllingPawn System Was Null"));
		return EBTNodeResult::Failed;
	}

	// AI가 이동 가능한 위치를 찾기 위해 네비게이션 시스템 객체를 가져오는 코드
	// AI가 순찰 지점을 찾으려면 NavMesh 위에서 유효한 좌표를 계산해야 함.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());

	if (nullptr == NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nav System Was Null"));
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);


	ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ICharacterAIInterface Inferface Cast Failed"));
		return EBTNodeResult::Failed;
	}

	FNavLocation NextPatrolPos;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, AIPawn->GetAIPatrolRange(), NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Type();
}
