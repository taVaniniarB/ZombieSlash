// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZSHUDWidget.h"
#include "HPBarWidget.h"
#include "Interface/CharacterHUDInterface.h"

UZSHUDWidget::UZSHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UZSHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �� �������� �̹� UI�� ���� �ʱ�ȭ�� ���� ���°�
	// �����Ϳ��� ��ġ�ص� ������ ���� �ʱ�ȭ���� ���̱� ������, �����Ϳ��� ������ ���� �̸� �״�� ����ؼ� ������ ���� �� �ִ�.
	// ���� ���� �Ŀ��� �̰Ϳ� �����ϱ� ���� ��������� �־��ش�.

	HPBar = Cast<UHPBarWidget>(GetWidgetFromName(TEXT("WidgetHPBar")));
	
	ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HPBar->SetOwningActor(GetOwningPlayerPawn());
		HUDPawn->SetupHUDWidget(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UZSHUDWidget -> NativeConstruct : ICharacterHUDInterface Cast Failed"));
	}
}

void UZSHUDWidget::UpdateHPBar(float NewCurHP)
{
	HPBar->UpdateHpBar(NewCurHP);
}

void UZSHUDWidget::UpdateStat(const FCharacterStat& InBaseStat, const FCharacterStat& InModifierStat)
{
	HPBar->UpdateStat(InBaseStat, InModifierStat);
}