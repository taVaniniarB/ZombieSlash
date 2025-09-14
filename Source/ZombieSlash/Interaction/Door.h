// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Door.generated.h"

UCLASS()
class ZOMBIESLASH_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	FPrimaryAssetId RequiredItem; // 문을 여는 데 필요한 아이템 (ex: 열쇠)

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// Interactable Interface
	virtual bool CanInteract_Implementation(AActor* Interactor) const override; // 상호작용자는 열쇠 아이템을 보유하고 있는가?
	virtual void Interact_Implementation(AActor* Interactor) override; // Tick 활성화, 문 액터 회전 위한 변수 세팅
	virtual FText GetDisplayMessage_Implementation() const override; // UI에 표시될 메시지 ("문 열기") 반환
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle; // 문이 열릴 각도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float RotationSpeed = 1.0f; // 문이 열리는 속도

protected:
	// Sphere Trigger에 바인드
	// 플레이어 폰(Interaction 컴포넌트)이 관리하는 OverlappingInteractables 배열에 이 객체를 추가한다.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	// Sphere Trigger에 바인드
	// 플레이어 폰(Interaction 컴포넌트)이 관리하는 OverlappingInteractables 배열에서 이 객체를 제거한다.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool bIsOpen = false; // 문이 열린 상태인가?
	FQuat InitialRotation; // 초기 회전 상태
	FQuat TargetRotation; // 목표 회전
	float CurRotationAlpha = 0.0f; // 문 열기 진행도 (0 ~ 1)
};
