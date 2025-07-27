// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Footstep.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	bool bIsLeftFoot; // �� ��Ƽ���̰� �޹߿����� ����

private:
	UPROPERTY(EditAnywhere, Category = Footstep)
	TObjectPtr<class UDataTable> FootstepSoundTable;

	// ���ڱ� ���� ���
	USoundBase* GetFootstepSound(USkeletalMeshComponent* MeshComp, FName BoneName);
};