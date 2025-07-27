// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class ZOMBIESLASH_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	AWeaponBase();

protected:
    // 무기 데이터에 대한 참조
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UWeaponData> WeaponData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class USceneComponent> SceneComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    TObjectPtr<UStaticMeshComponent> WeaponMeshComponent;

public:
    // 무기 초기화 - 데이터로부터 설정값들을 가져옴
    virtual void InitializeWeapon(UWeaponData* InWeaponData, AActor* InOwner);

    // 공통 인터페이스
    virtual void OnEquip();
    virtual void OnUnequip();
    virtual void StartAttack() {}
    virtual void StopAttack() {}
    virtual bool CanAttack() const { return true; }

    UWeaponData* GetWeaponData() const { return WeaponData; }

    FName SocketName;
};
