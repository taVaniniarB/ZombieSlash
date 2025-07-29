// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
    AMeleeWeapon();

protected:
    virtual void OnEquip() override;
    virtual void OnUnequip() override;


//protected:
//    float AttackRange;
//    float AttackRadius;
//    float AttackDamage;

protected:
    // �޺� �ý��� ���� ������
    UPROPERTY(BlueprintReadOnly, Category = "Combo")
    int32 CurCombo = 0;

    bool bHasNextComboCommand;

    FTimerHandle ComboTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPriavateAccess = "true"))
    TObjectPtr<class UComboActionData> ComboActionData;

    // �ִϸ��̼� ����
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> ComboActionMontage;

public:
    virtual void StartAttack() override;
    //virtual bool CanAttack() const override;

    // �������� ���� ��ɵ�
    void ProcessComboCommand();
    void AttackHitCheck();

private:
    void ComboActionBegin();
    void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
    void SetComboCheckTimer();
    void ComboCheck();

    // �������� �����ͷ� ĳ�����ؼ� ���
    const class UMeleeData* GetMeleeWeaponData() const { return Cast<UMeleeData>(WeaponData); }
};
