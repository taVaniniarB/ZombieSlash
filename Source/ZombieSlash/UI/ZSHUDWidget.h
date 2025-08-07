// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/WeaponType.h"
#include "ZSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UZSHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZSHUDWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void HandleWeaponEquipped(EWeaponType WeaponType);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHPBar(float NewCurHP);
	void UpdateStat(const struct FCharacterStat& InBaseStat, const struct FCharacterStat &InModifierStat);
	void UpdateAmmo(float CurAmmo, float MaxAmmo);

protected:
	UPROPERTY()
	TObjectPtr<class UHPBarWidget> HPBar;

	UPROPERTY()
	TObjectPtr<class UAmmoWidget> AmmoWidget;

private:
	void ShowAmmoWidget();
	void HideAmmoWidget();
};
