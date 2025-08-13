// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Door.h"
#include "Components/SphereComponent.h"
#include "Physics/ZSCollision.h"
#include "Interface/CharacterInteractInterface.h"

// Sets default values
ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	Trigger->SetSphereRadius(150.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;
	Trigger->SetupAttachment(Mesh);

	Trigger->SetCollisionProfileName(CPROFILE_ZSTRIGGER);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialRotation = GetActorQuat();
	TargetRotation = InitialRotation;
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 진행도가(0~1) 1 미만일 시
	if (CurRotationAlpha < 1.0f)
	{
		// DT * 속도에 따른 진행도 업데이트
		CurRotationAlpha = FMath::Clamp(CurRotationAlpha + DeltaTime * RotationSpeed, 0.0f, 1.0f);

		if (CurRotationAlpha >= 1.0f)
		{
			CurRotationAlpha = 1.0f;
			SetActorTickEnabled(false); // Tick 비활성화
		}

		// 업데이트된 진행도를 기반으로 구형 선형 보간
		FQuat NewRotation = FQuat::Slerp(InitialRotation, TargetRotation, CurRotationAlpha);
		SetActorRotation(NewRotation);
	}
}

bool ADoor::CanInteract_Implementation(AActor* Interactor) const
{
	if (bIsOpen) return false; // 이미 열린 상태인 경우 False 반환

	// 문을 열 수 있는 아이템(RequiredItem)이 설정되지 않았을 시 그냥 열린다
	if (!RequiredItem.IsValid()) return true;

	// 폰이 RequiredItem을 보유중일 시 문이 열린다
	ICharacterInteractInterface* Pawn = Cast<ICharacterInteractInterface>(Interactor);
	return Pawn->Execute_HasItem(Cast<UObject>(Pawn), RequiredItem);
}

void ADoor::Interact_Implementation(AActor* Interactor)
{
	SetActorTickEnabled(true);

	bIsOpen = true;
	InitialRotation = GetActorQuat();
	FQuat DeltaRotation = FQuat(FVector::UpVector, FMath::DegreesToRadians(OpenAngle));
	TargetRotation = DeltaRotation * InitialRotation;
	CurRotationAlpha = 0.0f;

	ICharacterInteractInterface* OverlappingPawn = Cast<ICharacterInteractInterface>(Interactor);
	if (OverlappingPawn)
	{
		OverlappingPawn->RemoveOverlappingInteractable(this);
	}
	Trigger->OnComponentBeginOverlap.RemoveAll(this);
	Trigger->OnComponentEndOverlap.RemoveAll(this);

	UE_LOG(LogTemp, Warning, TEXT("Door Open"));
}

FText ADoor::GetDisplayMessage_Implementation() const
{
	return FText::FromString(TEXT("문 열기"));
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICharacterInteractInterface* OverlappingPawn = Cast<ICharacterInteractInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->AddOverlappingInteractable(this);
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ICharacterInteractInterface* OverlappingPawn = Cast<ICharacterInteractInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->RemoveOverlappingInteractable(this);
	}
}

