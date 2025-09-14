// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "BTService_Detect.h"
#include "ZSAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CharacterAIInterface.h"
#include "AIController.h"
#include "Physics/ZSCollision.h"
#include "Engine/OverlapResult.h"

UBTService_Detect::UBTService_Detect()
{
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) return;

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!World) return;

	ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(ControllingPawn);
	if (!AIPawn) return;

	// NPC 인터페이스로부터 감지 영역 가져오기
	float DetectRadius = AIPawn->GetAIDetectRange();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn); // Trace Tag, complex 충돌감지 X, 자기 자신 무시

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, Center, FQuat::Identity, CCHANNEL_ZSACTION, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			if (Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				/*DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);*/
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}
