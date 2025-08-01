// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AnimationAttackInterface.h"
#include "Interface/CharacterWidgetInterface.h"
#include "Interface/CharacterStatInterface.h"
#include "Interface/CharacterWeaponInterface.h"
#include "CharacterBase.generated.h"


UCLASS()
class ZOMBIESLASH_API ACharacterBase : public ACharacter, 
	public IAnimationAttackInterface, public ICharacterWidgetInterface, public ICharacterStatInterface, public ICharacterWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	virtual void PostInitializeComponents() override;

	// Attack Hit Section
protected:
	virtual void AttackHitCheck() override; // 인터페이스로부터 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	void SetRunMode();
	bool bRunMode = false;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead(); // 죽는 상태 구현
	void PlayDeadAnimation(); // 몽타주 재생

	// Stat Section
protected:
	// 이 캐릭터의 스탯을 찾을 때 사용할 고유 ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ID)
	FName CharacterID;
	FName GetCharacterID() const { return CharacterID; }
	void SetCharacterID(FName InID) { CharacterID = InID; }

	virtual void SetupCharacterWidget(class UZSUserWidget* InWidget) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInventoryComponent> Inventory;

	// Weapon Interface
protected:
	virtual struct FCharacterStat GetWeaponOwnerStat() const override;
	virtual class AController* GetWeaponOwnerController() const override { return GetController(); };
	virtual class UAnimInstance* GetWeaponOwnerAnimInstance() const override { return GetMesh()->GetAnimInstance(); }
	virtual FVector GetWeaponOwnerLocation() const override { return GetActorLocation(); }
	virtual FVector GetWeaponOwnerForwardVector() const { return GetActorForwardVector(); }
	virtual float GetWeaponOwnerCapsuleRadius() const override;
	virtual void EndReload() override { return; }
	virtual void Parry() override { return; }
	virtual class UInventoryComponent* GetInventory() const override { return Inventory; }
	virtual bool GetShouldMove() const override;
};
