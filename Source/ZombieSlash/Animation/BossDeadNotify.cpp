// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BossDeadNotify.h"
#include "BossDeadNotify.h"
#include "Character/CharacterBoss.h"

void UBossDeadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    ACharacterBoss* Boss = Cast<ACharacterBoss>(MeshComp->GetOwner());
    if (Boss)
    {
        Boss->OnBossDead();
    }
}
