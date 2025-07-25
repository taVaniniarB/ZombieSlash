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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    TObjectPtr<UStaticMeshComponent> WeaponMeshComponent;


    // 플레이어캐릭터에 IWeaponInterface 장착시켜서 Getter 구현

    // 캐릭터에 무기액터 부착(스폰) 구현
    // UPROPERTY(EditDefaultsOnly)
    //TSubclassOf<class AWeaponBase> WeaponClass;
    //UPROPERTY()
    ///TObjectPtr<class AWeaponBase> Weapon;
    //---
    //Begin Play
    // Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
    // 이후 Weapon의 타입에 따라 다른 소켓에 붙도록 한다
    // Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Weapon->GetSocketName());
    // Weapon->SetOwner(this);
    // 캐릭터 공격시 캐릭터베이스의 함수 말고 Weapon의 함수 사용하도록 변경 후 테스트
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
