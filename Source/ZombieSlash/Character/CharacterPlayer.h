// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/CharacterItemInterface.h"
#include "CharacterPlayer.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIESLASH_API ACharacterPlayer : public ACharacterBase, public ICharacterItemInterface
{
	GENERATED_BODY()

public:
	ACharacterPlayer();

protected:
	virtual void BeginPlay() override;
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


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Attack();
	void PickupItem();
	void UseHealItem();

	void EquippedWeaponChanged();

	//ECharacterControlType CurrentCharacterControlType;

protected:
	// Item Section
	void AddOverlappingItem(class AItemPickup* InItemData);
	void RemoveOverlappingItem(class AItemPickup* InItemData);
	TArray<class AItemPickup*> OverlappingItems;
	TObjectPtr<AItemPickup> NearbyItem;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;


	// HUD Section
protected:
	//virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget) override;

protected:
	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target)
	TObjectPtr<AActor> AttackTarget;
};
