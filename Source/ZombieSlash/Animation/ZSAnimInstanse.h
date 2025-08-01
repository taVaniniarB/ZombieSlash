// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/WeaponType.h"
#include "Interface/WeaponAnimInterface.h"
#include "ZSAnimInstanse.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UZSAnimInstanse : public UAnimInstance, public IWeaponAnimInterface
{
	GENERATED_BODY()
public:
	UZSAnimInstanse();


	void SetRunMode(bool bRunMode) { bIsRunMode = bRunMode; }
	void SetCurWeaponType(EWeaponType InWeaponType) { CurWeaponType= InWeaponType; }
	void SetIsAiming(uint8 InIsAiming) { bIsAiming = InIsAiming; }
	void SetIsReloading(uint8 InIsReloading) { bIsReloading = InIsReloading; }
	void SetIsZooming(uint8 InIsZooming) { bIsZooming = InIsZooming; }
	void Shoot();
	FTimerHandle ShootAnimTimer;
	void ResetIsShooting() { bIsShooting = false; }

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRunMode : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bShouldMove : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsAiming : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsReloading : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsZooming : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsShooting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	EWeaponType CurWeaponType;
};
