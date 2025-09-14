// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/CharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ControllingPawn System Was Null"));
		return EBTNodeResult::Failed;
	}

	ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ICharacterAIInterface Inferface Cast Failed"));
		return EBTNodeResult::Failed;
	}

	// 'Attack'은 바로 끝나는 액션이 아니다.
	// 공격 시작->몽타주 재생->몽타주 끝->공격 끝
	// 즉 공격 명령을 내렸다고 태스크의 공격 명령을 '성공'이라 리턴할 수 없다
	// 태스크에서는, 공격 명령 -> InProgress 반환 -> 공격이 끝나면 Succeded 반환해야 한다.
	// 공격 끝나는 타이밍 체크 필요 // 델리게이트로 만들자


	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	// 이 델리게이트는 공격 애니메이션 몽타주 종료 시점에 Execute된다.
	AIPawn->SetAttackFinishedDelegate(OnAttackFinished);
	AIPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
