// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CharacterAIInterface.h"
#include "ZSAI.h"

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!TargetPawn) return EBTNodeResult::Failed;

	ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(Pawn);
	if (!AIPawn) return EBTNodeResult::Failed;

	float TurnSpeed = AIPawn->GetAITurnSpeed();
	if (TurnSpeed <= 0.0f)
	{
		return EBTNodeResult::Succeeded; // 현재 회전 유지
	}

	FRotator CurRot = Pawn->GetActorRotation();
	
	FVector TargetPawnPos = TargetPawn->GetActorLocation();
	FVector PawnPos = Pawn->GetActorLocation();
	FVector LookVector = TargetPawnPos - PawnPos;
	LookVector.Z = 0.0f;

	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	

	Pawn->SetActorRotation(FMath::RInterpTo(CurRot, TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
