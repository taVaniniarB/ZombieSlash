// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "ItemPickup.generated.h"


UCLASS()
class ZOMBIESLASH_API AItemPickup : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup();

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FPrimaryAssetId ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 Quantity;

public:
	// Interactable Interface
	virtual bool CanInteract_Implementation(AActor* Interactor) const override { return true; }
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetDisplayMessage_Implementation() const override;

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
};