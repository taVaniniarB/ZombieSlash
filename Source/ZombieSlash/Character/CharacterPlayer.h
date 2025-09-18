// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/CharacterInteractInterface.h"
#include "Interface/CharacterHUDInterface.h"
#include "Enums/WeaponType.h"
#include "Enums/WeaponState.h"
#include "CharacterPlayer.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIESLASH_API ACharacterPlayer : public ACharacterBase, public ICharacterInteractInterface, public ICharacterHUDInterface
{
	GENERATED_BODY()

public:
	ACharacterPlayer();
	
	virtual void Tick(float DeltaTime) override;

	virtual bool PickupItem(FPrimaryAssetId ItemID, EItemType ItemType, int32 Quantity) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	uint8 IsZooming() const { return bIsZooming; }
	uint8 IsAiming() const { return bIsAiming; }
	uint8 IsReloading() const { return bIsReloading; }

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
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> QuickSlotX;


	void Move(const FInputActionValue& Value);
	void ExitMontage();
	void Look(const FInputActionValue& Value);

	virtual void Attack() override;

	void Interact();
	UFUNCTION()
	virtual bool HasItem_Implementation(FPrimaryAssetId ItemID) override;
	void UseQuickSlotX();

	// Gun
public:
	void SetGunState(EGunState GunState, uint8 InIsZooming, bool PlayMontage);
	// Zoom, 총의 공격 입력이 있을 때 호출된다
	void ResetExitAimTimer();

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

public:
	void UpdateWeaponIMC(EWeaponType NewWeaponType);

protected:
	void ActiveCombatAction(bool bActive);

	// Interaction Section
protected:
	TObjectPtr<class UInteractionComponent> Interaction;
	UFUNCTION(BlueprintCallable)
	AActor* GetClosestInteractable();
	// 파사드
	void AddOverlappingInteractable(AActor* InInteractable);
	void RemoveOverlappingInteractable(AActor* InInteractable);

	// Inventory Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UQuickSlot> QuickSlot;

	// Weapon Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UPlayerWeaponManagerComponent> WeaponManager;

	UFUNCTION()
	void SwitchWeapon(const FInputActionInstance& Value);

public:
	void HandleAmmoChanged(int32 NewAmmo, int32 MaxAmmo);
	
	// HUD Section
protected:
	virtual void SetupHUDWidget(class UZSHUDWidget* InHUDWidget) override;
	virtual void BindWeaponEquippedEvent(UZSHUDWidget* InHUDWidget) override;

	// Attack Target
protected:
	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target)
	TObjectPtr<AActor> AttackTarget;

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UCameraControlComponent> CameraController;
};
