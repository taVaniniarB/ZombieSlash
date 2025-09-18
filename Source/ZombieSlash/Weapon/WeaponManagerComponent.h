// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class AWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedSignature, EWeaponType, WeaponType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponManagerComponent();

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponEquippedSignature OnWeaponEquipped;

public:
	// Called when the game starts
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    virtual void InitializeWeapons();

    UFUNCTION(BlueprintCallable)
    virtual void EquipWeaponByIndex(int32 Index);

    UFUNCTION(BlueprintCallable)
    virtual bool Reload();

    UFUNCTION(BlueprintCallable)
    virtual void Attack();

    UFUNCTION(BlueprintCallable)
    virtual bool CanAttack() const;

    UFUNCTION(BlueprintCallable)
    AWeaponBase* GetCurrentWeapon() const;

    UFUNCTION(BlueprintCallable)
    EWeaponType GetCurrentWeaponType() const;

    UFUNCTION(BlueprintCallable)
    int32 GetCurrentWeaponIndex() const { return CurWeaponIndex; }

    UFUNCTION(BlueprintCallable)
    const TArray<AWeaponBase*>& GetWeapons() const { return Weapons; }

protected:
    // 무기 액터 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<TObjectPtr<AWeaponBase>> Weapons;

    // 현재 무기 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 CurWeaponIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponSlotCount = 1;

	UPROPERTY()
	TObjectPtr<class ACharacterBase> OwnerCharacter;

protected:
    virtual void AttachWeaponToOwner(AWeaponBase* Weapon);

private:
    void SetOwnerCharacter();
};
