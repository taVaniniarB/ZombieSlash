// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPickup.h"
#include "Components/SphereComponent.h"
#include "Physics/ZSCollision.h"
#include "Engine/HitResult.h"
#include "Interface/CharacterItemInterface.h"

// Sets default values
AItemPickup::AItemPickup()
{
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ZSTRIGGER);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	/*if (nullptr == ItemData)
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("ItemPickup: ItemData was Null"));
		return;
	}*/

	ICharacterItemInterface* OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->AddOverlappingItem(this);
	}
}

void AItemPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ICharacterItemInterface* OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->RemoveOverlappingItem(this);
	}
}

