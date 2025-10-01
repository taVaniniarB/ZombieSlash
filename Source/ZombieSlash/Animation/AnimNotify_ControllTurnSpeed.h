// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ControllTurnSpeed.generated.h"

// 모션 워핑이 낫지 않을까?

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UAnimNotify_ControllTurnSpeed : public UAnimNotify
{
	GENERATED_BODY()
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float TurnSpeed;
};
