// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_DisableMovement.h"

void UAnimNotify_DisableMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
		if (Pawn)
		{
			if (AController* PC = Pawn->GetController())
			{
				PC->SetIgnoreMoveInput(true);
			}
		}
	}
}
