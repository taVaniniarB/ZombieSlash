// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_CameraShake.h"
//#include "Interface/CameraShakeInterface.h"
#include "Subsystem/GameplayEventSubsystem.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetWorld())
	{
		if (auto* Subsystem = UGameplayEventSubsystem::Get(MeshComp->GetWorld()))
		{
			Subsystem->OnCameraShakeRequested.Broadcast(ShakeType, Scale);
		}
		//UGameplayEventSubsystem::Get(MeshComp->GetWorld())->OnCameraShakeRequested.Broadcast(ShakeType, Scale);

		// Interface 사용 시 코드
		/*ICameraShakeInterface* Pawn = Cast<ICameraShakeInterface>(MeshComp->GetOwner());
		if (Pawn)
		{
			Pawn->ShakeCamera(ShakeType, Scale);
			UE_LOG(LogTemp, Warning, TEXT("Shake by Notify"));
		}*/
	}
}
