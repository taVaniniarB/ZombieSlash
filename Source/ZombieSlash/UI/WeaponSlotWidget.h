// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWeaponSlot> OwnerInventory;

	virtual void NativeDestruct() override;
};
