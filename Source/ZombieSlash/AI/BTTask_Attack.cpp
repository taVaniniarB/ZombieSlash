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

	// 'Attack'�� �ٷ� ������ �׼��� �ƴϴ�.
	// ���� ����->��Ÿ�� ���->��Ÿ�� ��->���� ��
	// �� ���� ����� ���ȴٰ� �½�ũ�� ���� ����� '����'�̶� ������ �� ����
	// �½�ũ������, ���� ��� -> InProgress ��ȯ -> ������ ������ Succeded ��ȯ�ؾ� �Ѵ�.
	// ���� ������ Ÿ�̹� üũ �ʿ� // ��������Ʈ�� ������


	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	// �� ��������Ʈ�� ���� �ִϸ��̼� ��Ÿ�� ���� ������ Execute�ȴ�.
	AIPawn->SetAttackFinishedDelegate(OnAttackFinished);
	AIPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
