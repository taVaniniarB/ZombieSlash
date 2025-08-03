// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/ComboActionData.h"
#include "Physics/ZSCollision.h"
#include "Engine/DamageEvents.h"
#include "UI/HPBarWidget.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "Animation/ZSAnimInstanse.h"
#include "Item/InventoryComponent.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(27.f, 88.0f);
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

	// Inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
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

void ACharacterBase::SetRunMode()
{
	bRunMode = !bRunMode;
	Cast<UZSAnimInstanse>(GetMesh()->GetAnimInstance())->SetRunMode(bRunMode);
	GetCharacterMovement()->MaxWalkSpeed = bRunMode ? 400.f : 300.f;
}

void ACharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;

	// Trace Tag: �ݸ��� �м��� ���� �ĺ���. ������ �۾��� ���� Attack�̶�� �±׷� ������ �� �ְ� ��
	// Trace Complex: ������ ����(���� concave)�� �ö󼭴� �͸� �����ѵ�, �̷� ������ �浹ü�� ������ �������� ���� �ɼ�
	// ������ ���� (�� ���, �ڱ� �ڽ�)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetTotalStat().AttackRadius;
	const float AttackDamage = Stat->GetTotalStat().Attack;

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
	GetMesh()->	GetAnimInstance()->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void ACharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	AnimInst->StopAllMontages(0.f);
	AnimInst->Montage_Play(DeadMontage, 1.0f);
}

// ������ ��������Ʈ�� HP�� �Լ� ���ε� �� �ʱ� ����
void ACharacterBase::SetupCharacterWidget(UZSUserWidget* InWidget)
{
	//HP�ٸ� �����Ѵ�
	UHPBarWidget* HPBarWidget = Cast<UHPBarWidget>(InWidget);

	if (HPBarWidget)
	{
		HPBarWidget->SetMaxHP(Stat->GetMaxHP());
		HPBarWidget->UpdateHpBar(Stat->GetCurHP());

		Stat->OnHPChanged.AddUObject(HPBarWidget, &UHPBarWidget::UpdateHpBar);
		Stat->OnStatChanged.AddUObject(HPBarWidget, &UHPBarWidget::UpdateStat);
	}
}

FCharacterStat ACharacterBase::GetWeaponOwnerStat() const
{
	return Stat->GetTotalStat();
}

float ACharacterBase::GetWeaponOwnerCapsuleRadius() const
{
	return GetCapsuleComponent()->GetScaledCapsuleRadius();
}

bool ACharacterBase::GetShouldMove() const
{
	float GroundSpeed = GetCharacterMovement()->Velocity.Size2D();
	
	return (GroundSpeed > 0) && (FVector::ZeroVector != GetCharacterMovement()->GetCurrentAcceleration());
}
