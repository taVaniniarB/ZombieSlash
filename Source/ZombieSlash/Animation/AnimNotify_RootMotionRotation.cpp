// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RootMotionRotation.h"
#include "Interface/AnimationAttackInterface.h"

void UAnimNotify_RootMotionRotation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IAnimationAttackInterface* Pawn = Cast<IAnimationAttackInterface>(MeshComp->GetOwner());
		if (Pawn)
		{
			//Pawn->ToggleRootMotionRotation(true);
		}
	}
}
