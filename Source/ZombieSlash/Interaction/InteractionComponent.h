// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBIESLASH_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TObjectPtr<AActor> ClosestInteractable;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddOverlappingInteractable(class AActor* InInteractable);
	void RemoveOverlappingInteractable(class AActor* InInteractable);

	UFUNCTION(BlueprintCallable)
	void TryInteract();

protected:
	void UpdateClosestInteractable();
	
	UPROPERTY()
	TArray<class AActor*> OverlappingInteractables;

	void ProccessInteraction(AActor* Interactable);
	void HandlePickup(AActor* Pickup);
	void HandleLockedDoor(AActor* Door);
	void HandleLootableCorpse(AActor* Corpse);

};
