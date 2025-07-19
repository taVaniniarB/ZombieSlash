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

	// Ÿ�̸Ӱ� ��ȿ�ϴ� = Ÿ�̸Ӱ� ���� �ִ� = �� ������ �Է��� ���� �޺��� �Ѿ �� �ִ� �Է����� ���
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

	AnimInst->Montage_Play(ComboActionMontage, 1.0f); // ����� ��Ÿ��, ��� �ӵ�

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACharacterBase::ComboActionEnd);
	AnimInst->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// ���� �޺� ���� ��, Ÿ�̸� �ڵ��� �ʱ�ȭ
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ACharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurCombo != 0); // �� �Լ��� ȣ��� �� ���� �޺��� 0�� �ƴ��� ����

	CurCombo = 0;

	if (AController* C = GetController())
	{
		C->SetIgnoreMoveInput(false);
	}
}

void ACharacterBase::SetComboCheckTimer()
{
	// ���� �� �޺��ΰ�? (�޺��׼� �����Ϳ����� �ε��� ��)
	int32 ComboIndex = CurCombo - 1;

	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	// ���� �޺� �Է����� �������� �ð�(��)
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	// �� ���� ������, ���� �޺��� ������ �޺��� �ƴϾ��ٴ� ��
	if (ComboEffectiveTime > 0.f)
	{
		// �ش� �ð� �� Ư�� �Լ��� ����ǵ��� 'SetTimer'�� �̿��Ѵ�.
		// SetTimer ����� ���� Ÿ�̸� �ڵ�� �Լ��� �ʿ��ϴ�.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

// Ÿ�̸� ���� �������� ȣ��
void ACharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate(); // Ÿ�̸� �ڵ� �ʱ�ȭ

	if (HasNextComboCommand) // Ÿ�̸Ӱ� ���� ���� �Է��� ���� ���
	{
		// �޺��� 1 �ø� ����, ��Ÿ�� ������ �Ѱ��ְ�, �ٽ� Ÿ�̸Ӹ� ������

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
	
	// Trace Tag: �ݸ��� �м��� ���� �ĺ���. ������ �۾��� ���� Attack�̶�� �±׷� ������ �� �ְ� ��
    // Trace Complex: ������ ����(���� concave)�� �ö󼭴� �͸� �����ѵ�, �̷� ������ �浹ü�� ������ �������� ���� �ɼ�
    // ������ ���� (�� ���, �ڱ� �ڽ�)
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
	////ĳ���� �Ӹ� �� HP�ٸ� �����Ѵ�
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
