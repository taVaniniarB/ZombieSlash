// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionComponent.h"
#include "Interface/Interactable.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateClosestInteractable();
}

void UInteractionComponent::AddOverlappingInteractable(AActor* InInteractable)
{
	OverlappingInteractables.Add(InInteractable);
	UE_LOG(LogTemp, Warning, TEXT("Overlapping Interacatble %d"), OverlappingInteractables.Num());
}

void UInteractionComponent::RemoveOverlappingInteractable(AActor* InInteractable)
{
	OverlappingInteractables.Remove(InInteractable);
	UE_LOG(LogTemp, Warning, TEXT("Overlapping Interacatble %d"), OverlappingInteractables.Num());
}

void UInteractionComponent::TryInteract()
{
	IInteractable* Interactable = Cast<IInteractable>(ClosestInteractable);
	if (!Interactable) return;

	// 직접 호출 대신 Execute_ 함수 사용
	if (IInteractable::Execute_CanInteract(ClosestInteractable, GetOwner()))
	{
		IInteractable::Execute_Interact(ClosestInteractable, GetOwner());
	}
}

void UInteractionComponent::UpdateClosestInteractable()
{
	ClosestInteractable = nullptr;
	float ClosestDist = FLT_MAX;

	for (AActor* Interactable : OverlappingInteractables)
	{
		if (!IsValid(Interactable)) continue;

		float Dist = FVector::Dist(GetOwner()->GetActorLocation(), Interactable->GetActorLocation());
		if (Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClosestInteractable = Interactable;
		}
	}
}