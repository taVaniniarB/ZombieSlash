// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EnableMovement.h"
#include "AnimNotify_EnableMovement.h"

void UAnimNotify_EnableMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
		if (Pawn)
		{
			if (AController* PC = Pawn->GetController())
			{
				PC->SetIgnoreMoveInput(false);
			}
		}
	}
}
