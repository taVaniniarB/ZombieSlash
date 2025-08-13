// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPickup.h"
#include "Components/SphereComponent.h"
#include "Physics/ZSCollision.h"
#include "Engine/HitResult.h"
#include "Interface/CharacterInteractInterface.h"
#include "Item/ItemData.h"
#include "GameData/ItemMetadata.h"
#include "Item/ItemManagerSubsystem.h"


AItemPickup::AItemPickup()
{
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	Trigger->SetSphereRadius(150.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ZSTRIGGER);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AItemPickup::OnOverlapEnd);

	Quantity = 1;
}

void AItemPickup::Interact_Implementation(AActor* Interactor)
{
	ICharacterInteractInterface* Pawn = Cast<ICharacterInteractInterface>(Interactor);
	if (Pawn)
	{
		UItemManagerSubsystem* ItemMgr = UItemManagerSubsystem::Get(this);
		FItemMetadata Metadata;
		ItemMgr->GetMetadata(ItemID, Metadata);
		if (Pawn->PickupItem(ItemID, Metadata.ItemType, Quantity))
		{
			Destroy();
			Pawn->RemoveOverlappingInteractable(this);
		}
	}
}

FText AItemPickup::GetDisplayMessage_Implementation() const
{
	if (UItemManagerSubsystem* ItemMgr = UItemManagerSubsystem::Get(this))
	{
		FItemMetadata Metadata;
		if (ItemMgr->GetMetadata(ItemID, Metadata))
		{
			return FText::Format(
				FText::FromString(TEXT("{0} 줍기")), FText::FromString(Metadata.DisplayName) // FString -> FText 변환
			);
		}
	}

	return FText();
}

void AItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICharacterInteractInterface* OverlappingPawn = Cast<ICharacterInteractInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->AddOverlappingInteractable(this);
	}
}

void AItemPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ICharacterInteractInterface* OverlappingPawn = Cast<ICharacterInteractInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->RemoveOverlappingInteractable(this);
	}
}

