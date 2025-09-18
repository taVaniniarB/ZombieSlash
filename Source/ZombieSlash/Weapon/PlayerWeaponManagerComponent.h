// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponManagerComponent.h"
#include "Enums/WeaponState.h"
#include "Enums/WeaponType.h"
#include "PlayerWeaponManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UPlayerWeaponManagerComponent : public UWeaponManagerComponent
{
	GENERATED_BODY()
public:
	UPlayerWeaponManagerComponent();

public:
    virtual void BeginPlay() override;
    virtual void EquipWeaponByIndex(int32 Index) override;
    virtual void Attack() override;

    // 플레이어 전용 함수
    UFUNCTION(BlueprintCallable)
    void SwitchWeapon(const FInputActionInstance& Value);

    UFUNCTION(BlueprintCallable)
    void EnterAimState();

    UFUNCTION(BlueprintCallable)
    void SetGunState(EGunState State, uint8 InIsZooming, bool PlayMontage);

private:
    void UpdateWeaponIMC(EWeaponType WeaponType);
    void SetPlayerOwner();

    TObjectPtr<class ACharacterPlayer> PlayerOwner;
};
