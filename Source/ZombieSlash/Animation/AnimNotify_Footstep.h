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
	bool bIsLeftFoot; // 이 노티파이가 왼발용인지 여부

private:
	UPROPERTY(EditAnywhere, Category = Footstep)
	TObjectPtr<class UDataTable> FootstepSoundTable;

	// 발자국 사운드 재생
	USoundBase* GetFootstepSound(USkeletalMeshComponent* MeshComp, FName BoneName);
};