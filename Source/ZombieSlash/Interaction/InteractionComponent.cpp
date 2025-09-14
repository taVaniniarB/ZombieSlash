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
}

void UInteractionComponent::RemoveOverlappingInteractable(AActor* InInteractable)
{
	OverlappingInteractables.Remove(InInteractable);
}

void UInteractionComponent::TryInteract()
{
	IInteractable* Interactable = Cast<IInteractable>(ClosestInteractable);
	if (!Interactable) return;

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