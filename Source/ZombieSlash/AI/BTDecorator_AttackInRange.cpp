// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "BTDecorator_AttackInRange.h"
#include "ZSAI.h"
#include "Interface/CharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AActor* Owner = OwnerComp.GetOwner();
	if (!Owner) return false;

	float AttackRange = 0.0f;
	if (ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(Owner))
	{
		AttackRange = AIPawn->GetAIAttackRange();
	}

	// Blackboard에 접근하여 Target 변수 가져오기
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target) return false;

	float Dist = Owner->GetDistanceTo(Target);

	return (Dist <= AttackRange);
}
