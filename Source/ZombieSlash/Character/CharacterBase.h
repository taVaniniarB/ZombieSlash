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
	// 이 캐릭터의 스탯을 찾을 때 사용할 고유 ID
	// 접근 제한 : 이 변수는 클래스 디폴트(Class Defaults) 에서만 편집할 수 있습니다. 즉, 콘텐츠 브라우저에서 이 클래스의 블루프린트 자식 클래스를 열었을 때 디테일(Details) 패널에서 값을 설정할 수 있습니다.
	// 런타임 수정 불가 : 게임이 실행 중일 때(런타임) 생성된 이 클래스의 인스턴스(월드에 배치된 캐릭터) 에서는 이 값을 편집할 수 없습니다.이는 캐릭터의 기본 스탯 ID는 게임 중에 변할 일이 없다는 전제에 적합합니다.
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
