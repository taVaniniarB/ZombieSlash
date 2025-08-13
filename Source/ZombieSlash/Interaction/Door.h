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
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	FPrimaryAssetId RequiredItem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Interactable Interface
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetDisplayMessage_Implementation() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float RotationSpeed;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool bIsOpen = false;
	FQuat InitialRotation;
	FQuat TargetRotation;
	float CurRotationAlpha = 0.0f; // 문 열기 진행도
};
