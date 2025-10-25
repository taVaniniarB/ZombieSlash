// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/ItemType.h"
#include "CharacterInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCharacterInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESLASH_API ICharacterInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*virtual void AddOverlappingInteractable(class AActor* InActor) = 0;
	virtual void RemoveOverlappingInteractable(class AActor* InActor) = 0;

	virtual bool PickupItem(FPrimaryAssetId ItemID, EItemType Type, int32 Quantity) = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool HasItem(FPrimaryAssetId ItemID);*/




	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void AddOverlappingInteractable(AActor* InActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void RemoveOverlappingInteractable(AActor* InActor);

	virtual bool PickupItem(FPrimaryAssetId ItemID, EItemType Type, int32 Quantity) = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool HasItem(FPrimaryAssetId ItemID);
};
