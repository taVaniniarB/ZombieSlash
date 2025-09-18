// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterNPC.h"
#include "UI/ZSWidgetComponent.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "AI/ZSAIController.h"

ACharacterNPC::ACharacterNPC()
{
	HPBar = CreateDefaultSubobject<UZSWidgetComponent>(TEXT("Widget"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetRef(TEXT("/Game/UI/WBP_HPBar.WBP_HPBar_C"));
	if (HPBarWidgetRef.Class)
	{
		HPBar->SetWidgetClass(HPBarWidgetRef.Class);
		HPBar->SetWidgetSpace(EWidgetSpace::Screen);
		HPBar->SetDrawSize(FVector2D(100.f, 8.f));
		HPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACharacterNPC::SetDead()
{
	Super::SetDead();

	AZSAIController* ZSAIController = Cast<AZSAIController>(GetController());
	if (ZSAIController)
	{
		ZSAIController->StopAI();
	}
	FTimerHandle DeadTimerHandle;

	// ��� �Լ� ���� �־��� �� ������, ���� ���ִ� �Լ��� ���� ����� �� ���ŷӴ�.
	// ���� �����ϰ� ���� �Լ��� ����Ѵ�.
	// Ÿ�̸� ��������Ʈ ����ü�� �������ѳ���, �̰��� �ѱ��.
	// �� ������ ���� ���� �Լ��� ����� Ÿ�̸� ��������Ʈ�� �Ｎ���� ���� ��������ش�.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}), 5.f, false);

	HPBar->SetHiddenInGame(true);
}

void ACharacterNPC::Attack()
{
	Super::Attack();

	if (AttackMontage)
	{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		AnimInst->Montage_Play(AttackMontage, Stat->GetTotalStat().AttackSpeed);
	}
}

float ACharacterNPC::GetAIPatrolRange()
{
	return 800.0f;
}

float ACharacterNPC::GetAIDetectRange()
{
	return 400.f;
}

float ACharacterNPC::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius();
}

float ACharacterNPC::GetAITurnSpeed()
{
	return 0.0f;
}

void ACharacterNPC::AttackByAI()
{
	Attack();
	UE_LOG(LogTemp, Warning, TEXT("Attack By AI"));

	if (OnAttackFinished.IsBound())
	{
		// ���� �ִϸ��̼��� ������ �� ȣ��� �Լ��� ��������Ʈ�� ���ε�
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				if (OnAttackFinished.IsBound())
				{
					OnAttackFinished.Execute();
				}
			});
		AnimInst->Montage_SetEndDelegate(EndDelegate);
	}
}

void ACharacterNPC::SetAttackFinishedDelegate(FAICharacterAttackFinished InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}


