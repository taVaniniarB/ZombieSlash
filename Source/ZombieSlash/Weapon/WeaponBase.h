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
    // ���� �����Ϳ� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UWeaponData> WeaponData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class USceneComponent> SceneComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    TObjectPtr<UStaticMeshComponent> WeaponMeshComponent;

    // ���콺�ٷ� ���� �ٲٴ� input �߰�
    // �ѱ� �ִϸ��̼� �ް� �� ���� ABP ����
    // ���� ������ ����� Spawn
    // ���� ���⿡ ���� OnEquip ȣ��, ���� �� �ٸ� ����� OnUnequip ȣ��
    // ���� ����ġ �� ������ On/Unequip ȣ��
    // �κ��丮 �� ���⽽�Կ� �������� �� ���� ������ ����� Destroy

public:
    // ���� �ʱ�ȭ - �����ͷκ��� ���������� ������
    virtual void InitializeWeapon(UWeaponData* InWeaponData, AActor* InOwner);

    // ���� �������̽�
    virtual void OnEquip();
    virtual void OnUnequip();
    virtual void StartAttack() {}
    virtual void StopAttack() {}
    virtual bool CanAttack() const { return true; }


    FName SocketName;
};
