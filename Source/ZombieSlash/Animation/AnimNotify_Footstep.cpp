// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Footstep.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/FootstepSoundData.h"

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComp was null"));
		return;
	}

	// 발의 뼈 이름 (예: 왼발 또는 오른발)
	FName BoneName = bIsLeftFoot ? TEXT("foot_l") : TEXT("foot_r");

	// 해당 뼈 위치에서 지형 체크 및 사운드 재생
	USoundBase* FootstepSound = GetFootstepSound(MeshComp, BoneName);

	if (FootstepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			MeshComp->GetOwner(),
			FootstepSound,
			MeshComp->GetBoneLocation(BoneName),
			1.0f,
			1.0f,
			0.0f,
			nullptr,
			nullptr
		);
	}
}

USoundBase* UAnimNotify_Footstep::GetFootstepSound(USkeletalMeshComponent* MeshComp, FName BoneName)
{
	if (!FootstepSoundTable)
		return nullptr;

	FVector Start = MeshComp->GetBoneLocation(BoneName) + FVector(0, 0, 10.0f); // 클리핑 현상 고려
	FVector End = Start - FVector(0, 0, 100.0f); // 발 아래로 레이캐스트

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());

	// 지형 체크를 위한 LineTrace
	bool bHit = MeshComp->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldStatic,
		QueryParams
	);

	if (bHit && HitResult.PhysMaterial.IsValid())
	{
		UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
		FName RowName = FName(*FString::FromInt((int32)PhysMaterial->SurfaceType.GetValue()));

		// DataTable에서 해당 SurfaceType에 맞는 사운드 배열 조회
		const FFootstepSoundData* Row = FootstepSoundTable->FindRow<FFootstepSoundData>(RowName, TEXT(""));
		if (Row && Row->FootstepSounds.Num() > 0)
		{
			// 랜덤 사운드 선택
			int32 RandomIndex = FMath::RandRange(0, Row->FootstepSounds.Num() - 1);

			return Row->FootstepSounds[RandomIndex];
		}
	}

	// 기본 사운드 반환 (Default SurfaceType)
	const FFootstepSoundData* DefaultRow = FootstepSoundTable->FindRow<FFootstepSoundData>(
		FName(*FString::FromInt((int32)SurfaceType_Default)), TEXT(""));

	return (DefaultRow && DefaultRow->FootstepSounds.Num() > 0) ? DefaultRow->FootstepSounds[0] : nullptr;
}
