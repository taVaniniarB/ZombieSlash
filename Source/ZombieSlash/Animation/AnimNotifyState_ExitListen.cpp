// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_ExitListen.h"
#include "Character/CharacterPlayer.h"

void UAnimNotifyState_ExitListen::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp) return;

	ACharacterPlayer* Player = Cast<ACharacterPlayer>(MeshComp->GetOwner());
	if (!Player) return;

	// 루트	모션 종료 허용
	Player->bCanExitRootMotion = true;
}

void UAnimNotifyState_ExitListen::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;
	ACharacterPlayer* Player = Cast<ACharacterPlayer>(MeshComp->GetOwner());
	if (!Player) return;

	Player->bCanExitRootMotion = false;
}
