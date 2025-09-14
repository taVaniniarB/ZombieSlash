// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterNPC.h"
#include "UI/ZSWidgetComponent.h"
#include "CharacterStat/CharacterStatComponent.h"

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

	FTimerHandle DeadTimerHandle;

	// 멤버 함수 만들어서 넣어줄 수 있지만, 액터 없애는 함수를 굳이 만드는 건 번거롭다.
	// 따라서 간편하게 람다 함수를 사용한다.
	// 타이머 델리게이트 구조체에 부착시켜놓고, 이것을 넘긴다.
	// 저 본문을 가진 람다 함수와 연결된 타이머 델리게이트를 즉석에서 만들어서 연결시켜준다.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}), 5.f, false);

	HPBar->SetHiddenInGame(true);
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


