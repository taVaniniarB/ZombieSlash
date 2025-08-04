// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventorySlot.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot", meta = (ExposeOnSpawn = "true"))
	FInventorySlot InventorySlot;

	UFUNCTION(BlueprintCallable)
	bool IsValid() { return InventorySlot.IsValid(); }
};
