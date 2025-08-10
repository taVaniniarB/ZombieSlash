// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enums/CameraShakeType.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camerashake")
	ECameraShakeType ShakeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camerashake")
	float Scale = 1.0f;
};
