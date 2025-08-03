// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/CharacterItemInterface.h"
#include "Interface/CharacterHUDInterface.h"
#include "Enums/WeaponType.h"
#include "CharacterPlayer.generated.h"

UENUM(BlueprintType)
enum class EGunState : uint8
{
	Ready = 0,
	Aim,
	Reload
};

UENUM(BlueprintType)
enum class EMeleeState : uint8
{
	Default = 0,
	Parry,
};


/**
 *
 */
UCLASS()
class ZOMBIESLASH_API ACharacterPlayer : public ACharacterBase, public ICharacterItemInterface, public ICharacterHUDInterface
{
	GENERATED_BODY()

public:
	ACharacterPlayer();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	uint8 GetIsAiming() const { return bIsAiming; }

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;
	
	// IMC CombatActions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputMappingContext> CombatIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> WeaponSwitchAction;

	// IMC Interaction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputMappingContext> InteractionIMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> PickupAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> QuickSlotX;


	void Move(const FInputActionValue& Value);
	void ExitMontage();
	void Look(const FInputActionValue& Value);

	void Attack();

	void PickupItem();
	void UseQuickSlotX();

	// Gun
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputMappingContext> GunIMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

	UPROPERTY(VisibleInstanceOnly, Category = Gun)
	EGunState CurGunState = EGunState::Ready;

	EGunState GetGunState() { return CurGunState; }
	void SetGunState(EGunState GunState, uint8 InIsZooming, bool PlayMontage);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gun)
	uint8 bIsZooming : 1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gun)
	uint8 bIsAiming : 1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gun)
	uint8 bIsReloading : 1;
	
	FTimerHandle ExitAimTimerHandle;
	float ExitAimTime = 3.0f;
	void EnterAimState();

	void UpdateAnimAimState();
	
	void CameraAimZoom();
	void ExitCameraAimZoom();

	// Zoom, 총의 공격 입력이 있을 때 호출된다
	void ResetExitAimTimer();
	void ExitAimState();

	UFUNCTION(BlueprintImplementableEvent, Category = Gun, Meta = (DisplayName = "OnGunStateChangedCpp"))
	void K2_OnGunStateChanged(EGunState NewSsate);
	void Reload();
	virtual void EndReload() override;

	// Melee
protected:
	EMeleeState CurMeleeStance = EMeleeState::Default;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputMappingContext> MeleeIMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> ParryAction;
	void Parry();

protected:
	void UpdateWeaponIMC(EWeaponType NewWeaponType);
	void ActiveCombatAction(bool bActive);

	// Inventory Section
protected:
	void AddOverlappingItem(class AItemPickup* InItemData);
	void RemoveOverlappingItem(class AItemPickup* InItemData);
	void UpdateClosestItem();
	TArray<class AItemPickup*> OverlappingItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TObjectPtr<AItemPickup> ClosestItem;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UQuickSlot> QuickSlot;
	
	// Weapon Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class AWeaponBase> CurWeapon; // 현재 캐릭터가 들고 있는 무기

	UFUNCTION()
	void SwitchWeapon(const FInputActionInstance& Value);
	
	// HUD Section
protected:
	virtual void SetupHUDWidget(class UZSHUDWidget* InHUDWidget) override;

	// Attack Target
protected:
	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target)
	TObjectPtr<AActor> AttackTarget;
};
