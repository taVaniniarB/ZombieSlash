// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AnimationAttackInterface.h"
#include "Interface/CharacterWidgetInterface.h"
#include "CharacterBase.generated.h"

UCLASS()
class ZOMBIESLASH_API ACharacterBase : public ACharacter, public IAnimationAttackInterface, public ICharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	// Attack Hit Section
protected:
	virtual void AttackHitCheck() override; // 인터페이스로부터 오버라이드
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
	// 이전에 입력 커맨드가 들어왔는가?
	// 내부 로직 구현에서만 사용할 변수이기에 UPROPERTY 하지 않음. 또한 그렇기게 uint8로 선언할 필요도 없다.
	bool HasNextComboCommand = false;


	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead(); // 죽는 상태 구현
	void PlayDeadAnimation(); // 몽타주 재생

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

	virtual void SetupCharacterWidget(UZSUserWidget* InUserWidget) override;

	// Weapon Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Weapon;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
