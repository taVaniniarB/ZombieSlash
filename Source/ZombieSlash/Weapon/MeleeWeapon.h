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
    // 콤보 시스템 관련 변수들
    UPROPERTY(BlueprintReadOnly, Category = "Combo")
    int32 CurCombo = 0;

    bool bHasNextComboCommand;

    FTimerHandle ComboTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPriavateAccess = "true"))
    TObjectPtr<class UComboActionData> ComboActionData;

    // 애니메이션 관련
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> ComboActionMontage;

public:
    virtual void StartAttack() override;
    //virtual bool CanAttack() const override;

    // 근접무기 전용 기능들
    void ProcessComboCommand();
    void AttackHitCheck();

private:
    void ComboActionBegin();
    void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
    void SetComboCheckTimer();
    void ComboCheck();

    // 근접무기 데이터로 캐스팅해서 사용
    const class UMeleeData* GetMeleeWeaponData() const { return Cast<UMeleeData>(WeaponData); }
};
