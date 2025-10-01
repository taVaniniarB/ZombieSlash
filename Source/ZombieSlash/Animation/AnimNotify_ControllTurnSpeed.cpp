// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ControllTurnSpeed.h"
#include "AnimNotify_ControllTurnSpeed.h"
#include "Interface/CharacterAIInterface.h"

void UAnimNotify_ControllTurnSpeed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ICharacterAIInterface* Pawn = Cast<ICharacterAIInterface>(MeshComp->GetOwner());
	if (Pawn)
	{
		Pawn->SetTurnSpeed(TurnSpeed);
	}
}
