// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterNPC.h"
#include "Components/WidgetComponent.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "UI/HPBarWidget.h"

ACharacterNPC::ACharacterNPC()
{
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetRef(TEXT("/Game/UI/WBP_HPBar.WBP_HPBar_C"));
	if (HPBarWidgetRef.Class)
	{
		HPBar->SetWidgetClass(HPBarWidgetRef.Class);
		HPBar->SetWidgetSpace(EWidgetSpace::Screen);
		HPBar->SetDrawSize(FVector2D(150.f, 15.f));
		HPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACharacterNPC::SetDead()
{
	Super::SetDead();

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

void ACharacterNPC::SetupCharacterWidget(UZSUserWidget* InUserWidget)
{
	Super::SetupCharacterWidget(InUserWidget);

	//ĳ���� �Ӹ� �� HP�ٸ� �����Ѵ�
	UHPBarWidget* HPBarWidget = Cast<UHPBarWidget>(InUserWidget);

	if (HPBarWidget)
	{
		HPBarWidget->SetMaxHP(Stat->GetMaxHP());
		HPBarWidget->UpdateHpBar(Stat->GetCurHP());

		Stat->OnHPChanged.AddUObject(HPBarWidget, &UHPBarWidget::UpdateHpBar);
	}
}
