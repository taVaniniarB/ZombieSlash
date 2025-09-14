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
	FPrimaryAssetId RequiredItem; // ���� ���� �� �ʿ��� ������ (ex: ����)

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// Interactable Interface
	virtual bool CanInteract_Implementation(AActor* Interactor) const override; // ��ȣ�ۿ��ڴ� ���� �������� �����ϰ� �ִ°�?
	virtual void Interact_Implementation(AActor* Interactor) override; // Tick Ȱ��ȭ, �� ���� ȸ�� ���� ���� ����
	virtual FText GetDisplayMessage_Implementation() const override; // UI�� ǥ�õ� �޽��� ("�� ����") ��ȯ
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle; // ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float RotationSpeed = 1.0f; // ���� ������ �ӵ�

protected:
	// Sphere Trigger�� ���ε�
	// �÷��̾� ��(Interaction ������Ʈ)�� �����ϴ� OverlappingInteractables �迭�� �� ��ü�� �߰��Ѵ�.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	// Sphere Trigger�� ���ε�
	// �÷��̾� ��(Interaction ������Ʈ)�� �����ϴ� OverlappingInteractables �迭���� �� ��ü�� �����Ѵ�.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool bIsOpen = false; // ���� ���� �����ΰ�?
	FQuat InitialRotation; // �ʱ� ȸ�� ����
	FQuat TargetRotation; // ��ǥ ȸ��
	float CurRotationAlpha = 0.0f; // �� ���� ���൵ (0 ~ 1)
};
