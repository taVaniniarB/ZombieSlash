// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "BTDecorator_AttackInRange.h"
#include "ZSAI.h"
#include "Interface/CharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

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
		return false;
	}

	float AttackRange = 0.0f;
	AttackRange = AIPawn->GetAIAttackRange();
	

	// Blackboard에 접근하여 Target 변수 가져오기
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target) return false;

	float Dist = ControllingPawn->GetDistanceTo(Target);

	return (Dist <= AttackRange);
}
