// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Ammo, Meta = (DisplayName = "OnAmmoChangedCpp"))
	void K2_OnAmmoUpdated(int32 CurAmmo, int32 MaxAmmo);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static FText IntToZeroPaddedString(int32 Number, int32 TotalLength);
};
