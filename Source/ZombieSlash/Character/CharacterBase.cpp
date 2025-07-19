// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/ComboActionData.h"
#include "Physics/ZSCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/CharacterStatComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CHARCAPSULE);

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Mesh
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Stat Component
	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));

	// Weapon Component
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHPZero.AddUObject(this, &ACharacterBase::SetDead);
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

void ACharacterBase::ProcessComboCommand()
{
	if (CurCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	// 타이머가 유효하다 = 타이머가 돌고 있다 = 이 시점의 입력은 다음 콤보로 넘어갈 수 있는 입력으로 취급
	if (ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = true;
	}
	else
	{
		HasNextComboCommand = false;
	}
}

void ACharacterBase::ComboActionBegin()
{
	CurCombo = 1;

	if (AController* C = GetController())
	{
		C->SetIgnoreMoveInput(true);
	}

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

	AnimInst->Montage_Play(ComboActionMontage, 1.0f); // 재생할 몽타주, 재생 속도

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACharacterBase::ComboActionEnd);
	AnimInst->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// 공격 콤보 시작 시, 타이머 핸들을 초기화
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ACharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurCombo != 0); // 이 함수가 호출될 땐 현재 콤보가 0이 아님을 보장

	CurCombo = 0;

	if (AController* C = GetController())
	{
		C->SetIgnoreMoveInput(false);
	}
}

void ACharacterBase::SetComboCheckTimer()
{
	// 현재 몇 콤보인가? (콤보액션 데이터에셋의 인덱스 상)
	int32 ComboIndex = CurCombo - 1;

	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	// 다음 콤보 입력으로 인정해줄 시간(초)
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	// 이 값이 양수라면, 현재 콤보가 마지막 콤보는 아니었다는 뜻
	if (ComboEffectiveTime > 0.f)
	{
		// 해당 시간 후 특정 함수가 실행되도록 'SetTimer'를 이용한다.
		// SetTimer 사용을 위해 타이머 핸들과 함수가 필요하다.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

// 타이머 종료 시점에서 호출
void ACharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate(); // 타이머 핸들 초기화

	if (HasNextComboCommand) // 타이머가 도는 동안 입력이 들어온 경우
	{
		// 콤보를 1 올린 다음, 몽타주 섹션을 넘겨주고, 다시 타이머를 돌린다

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

		CurCombo = FMath::Clamp(CurCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurCombo);
		AnimInst->Montage_JumpToSection(NextSection, ComboActionMontage);
		
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void ACharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	
	// Trace Tag: 콜리전 분석할 때의 식별자. 수행할 작업에 대해 Attack이라는 태그로 조사할 수 있게 함
    // Trace Complex: 복잡한 형태(오목 concave)는 올라서는 것만 가능한데, 이런 복잡한 충돌체도 감지할 것인지에 대한 옵션
    // 무시할 액터 (이 경우, 자기 자신)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	
	const float AttackRange = 45.f;
	const float AttackRadius = 50.f;
	const float AttackDamage = 30.f;

	const FVector Start = GetActorLocation() + GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius());
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult, Start, End,
		FQuat::Identity, CCHANNEL_ZSACTION, 
		FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + 0.5 * (End - Start);
	float CapsuleHalfHeight = (AttackRange * 0.5) + AttackRadius;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor,
		false, 3.f);
#endif
}

void ACharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void ACharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	AnimInst->StopAllMontages(0.f);
	AnimInst->Montage_Play(DeadMontage, 1.0f);
}

void ACharacterBase::SetupCharacterWidget(UZSUserWidget* InUserWidget)
{
	////캐릭터 머리 위 HP바를 세팅한다
	//UHPBarWidget* HPBarWidget = Cast<UHPBarWidget>(InUserWidget);
	//
	//if (HPBarWidget)
	//{
	//	HPBarWidget->SetMaxHP(Stat->GetMaxHP());
	//	HPBarWidget->UpdateHpBar(Stat->GetCurHP());

	//	Stat->OnHPChanged.AddUObject(HPBarWidget, UHPBarWidget::UpdateHpBar);
	//}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
