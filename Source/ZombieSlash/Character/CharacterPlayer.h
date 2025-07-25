// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/CharacterItemInterface.h"
#include "Interface/CharacterHUDInterface.h"
#include "CharacterPlayer.generated.h"

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
	virtual void PostInitializeComponents() override;

	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Character Control Section
protected:
	//void ChangeCharacterControl();
	//void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	//virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;


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
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> PickupAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> HealAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> WeaponSwitchAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Attack();

	void PickupItem();
	void UseHealItem();

	//ECharacterControlType CurrentCharacterControlType;

protected:
	// Inventory Section
	void AddOverlappingItem(class AItemPickup* InItemData);
	void RemoveOverlappingItem(class AItemPickup* InItemData);
	void UpdateClosestItem();
	TArray<class AItemPickup*> OverlappingItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TObjectPtr<AItemPickup> ClosestItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInventoryComponent> Inventory;
	
	// Weapon Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class AWeaponBase> CurWeapon; // 현재 캐릭터가 들고 있는 무기

	UFUNCTION()
	void SwitchWeapon(const FInputActionInstance& Value);

	UFUNCTION()
	void HandleWeaponChanged(const UWeaponData* Weapon);
	
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
