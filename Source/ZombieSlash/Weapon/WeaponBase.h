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

    // 마우스휠로 무기 바꾸는 input 추가
    // 총기 애니메이션 받고 총 전용 ABP 제작
    // 새로 장착한 무기들 Spawn
    // 현재 무기에 대해 OnEquip 호출, 슬롯 내 다른 무기는 OnUnequip 호출
    // 무기 스위치 시 적절히 On/Unequip 호출
    // 인벤토리 내 무기슬롯에 변동있을 시 기존 슬롯의 무기들 Destroy

public:
    // 무기 초기화 - 데이터로부터 설정값들을 가져옴
    virtual void InitializeWeapon(UWeaponData* InWeaponData, AActor* InOwner);

    // 공통 인터페이스
    virtual void OnEquip();
    virtual void OnUnequip();
    virtual void StartAttack() {}
    virtual void StopAttack() {}
    virtual bool CanAttack() const { return true; }


    FName SocketName;
};
