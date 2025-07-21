// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPBarWidget.h"
#include "Components/ProgressBar.h"
#include "HPBarWidget.h"
#include "Interface/CharacterWidgetInterface.h"

// �𸮾󿡼� FObjectInitializer�� ����ϴ� ����:
// UObject ��� Ŭ������ ���� Ÿ�ְ̹� �޸� �ʱ�ȭ�� �Ϲ� C++�� �ٸ�
// �ʱ�ȭ ������ ��Ȯ�� �ϱ� ���� Super(ObjectInitializer)�� �ʱ�ȭ ����Ʈ���� �ݵ�� ȣ���ϴ� ����
UHPBarWidget::UHPBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxHP = -1.f;
}

// �����ڿ����� ���� ���� Ʈ��(�����̳ʿ��� ���� ��ư, ���α׷��� �� ��)�� �������� �ʾҴ�. ���� ���α׷��� �� �����ʹ� NativeConstruct���� ä���ش�.
void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBHPBar")));

	// �⺻������ ������ ���� ��ü�� ĳ���ͺ��̽�. �׷��ٸ� OwningActor�� ĳ���ͺ��̽��� ĳ�����ؾ� �ұ�?
	// �׷��� �Ѵٸ� �� HP�� ������ ĳ���͸� ���� �� �ְ� �Ǿ������. �������� ����� ���� �����ϱ� ���� �������̽� �������
	ICharacterWidgetInterface* Character = Cast<ICharacterWidgetInterface>(OwningActor);
	if (Character)
	{
		Character->SetupCharacterWidget(this);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("NativeConstruct : ICharacterWidgetInterface Cast Failed"));
}

void UHPBarWidget::UpdateStat(const FCharacterStat& BaseStat, const FCharacterStat& ModifierStat)
{
	MaxHP = (BaseStat + ModifierStat).MaxHP;
	
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(CurHP / MaxHP);
	}
}

void UHPBarWidget::UpdateHpBar(float NewCurHP)
{
	CurHP = NewCurHP;

	ensure(MaxHP > 0.f);

	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurHP / MaxHP);
	}
}
