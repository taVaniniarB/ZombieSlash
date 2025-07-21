// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AnimationAttackInterface.h"
#include "Interface/CharacterWidgetInterface.h"
#include "Interface/CharacterStatInterface.h"
#include "CharacterBase.generated.h"

UCLASS()
class ZOMBIESLASH_API ACharacterBase : public ACharacter, 
	public IAnimationAttackInterface, public ICharacterWidgetInterface, public ICharacterStatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	virtual void PostInitializeComponents() override;

	// Attack Hit Section
protected:
	virtual void AttackHitCheck() override; // �������̽��κ��� �������̵�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;
	void ProcessComboCommand();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPriavateAccess = "true"))
	TObjectPtr<class UComboActionData> ComboActionData;

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurCombo = 0;
	FTimerHandle ComboTimerHandle;
	// ������ �Է� Ŀ�ǵ尡 ���Դ°�?
	// ���� ���� ���������� ����� �����̱⿡ UPROPERTY ���� ����. ���� �׷���� uint8�� ������ �ʿ䵵 ����.
	bool HasNextComboCommand = false;


	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead(); // �״� ���� ����
	void PlayDeadAnimation(); // ��Ÿ�� ���

	// Stat Section
protected:
	// �� ĳ������ ������ ã�� �� ����� ���� ID
	// ���� ���� : �� ������ Ŭ���� ����Ʈ(Class Defaults) ������ ������ �� �ֽ��ϴ�. ��, ������ ���������� �� Ŭ������ �������Ʈ �ڽ� Ŭ������ ������ �� ������(Details) �гο��� ���� ������ �� �ֽ��ϴ�.
	// ��Ÿ�� ���� �Ұ� : ������ ���� ���� ��(��Ÿ��) ������ �� Ŭ������ �ν��Ͻ�(���忡 ��ġ�� ĳ����) ������ �� ���� ������ �� �����ϴ�.�̴� ĳ������ �⺻ ���� ID�� ���� �߿� ���� ���� ���ٴ� ������ �����մϴ�.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stat)
	FName CharacterID;
	virtual FName GetCharacterID() const override { return CharacterID; }

	virtual void SetupCharacterWidget(class UZSUserWidget* InWidget) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

	// Weapon Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Weapon;

};
