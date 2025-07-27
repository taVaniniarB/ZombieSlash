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

	// ���� �� �̸� (��: �޹� �Ǵ� ������)
	FName BoneName = bIsLeftFoot ? TEXT("foot_l") : TEXT("foot_r");

	// �ش� �� ��ġ���� ���� üũ �� ���� ���
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

	FVector Start = MeshComp->GetBoneLocation(BoneName) + FVector(0, 0, 10.0f); // Ŭ���� ���� ���
	FVector End = Start - FVector(0, 0, 100.0f); // �� �Ʒ��� ����ĳ��Ʈ

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());

	// ���� üũ�� ���� LineTrace
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

		// DataTable���� �ش� SurfaceType�� �´� ���� �迭 ��ȸ
		const FFootstepSoundData* Row = FootstepSoundTable->FindRow<FFootstepSoundData>(RowName, TEXT(""));
		if (Row && Row->FootstepSounds.Num() > 0)
		{
			// ���� ���� ����
			int32 RandomIndex = FMath::RandRange(0, Row->FootstepSounds.Num() - 1);

			return Row->FootstepSounds[RandomIndex];
		}
	}

	// �⺻ ���� ��ȯ (Default SurfaceType)
	const FFootstepSoundData* DefaultRow = FootstepSoundTable->FindRow<FFootstepSoundData>(
		FName(*FString::FromInt((int32)SurfaceType_Default)), TEXT(""));

	return (DefaultRow && DefaultRow->FootstepSounds.Num() > 0) ? DefaultRow->FootstepSounds[0] : nullptr;
}
