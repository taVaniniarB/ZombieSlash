// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZSHUDWidget.h"
#include "HPBarWidget.h"
#include "Interface/CharacterHUDInterface.h"
#include "AmmoWidget.h"

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
	AmmoWidget = Cast<UAmmoWidget>(GetWidgetFromName(TEXT("WidgetAmmo")));

	ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HPBar->SetOwningActor(GetOwningPlayerPawn());
		HUDPawn->SetupHUDWidget(this);
		HUDPawn->BindWeaponEquippedEvent(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UZSHUDWidget -> NativeConstruct : ICharacterHUDInterface Cast Failed"));
	}
}

void UZSHUDWidget::HandleWeaponEquipped(EWeaponType WeaponType)
{
	if (WeaponType == EWeaponType::Gun)
	{
		ShowAmmoWidget();
	}
	else
	{
		HideAmmoWidget();
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

void UZSHUDWidget::UpdateAmmo(float CurAmmo, float MaxAmmo)
{
	AmmoWidget->K2_OnAmmoUpdated(CurAmmo, MaxAmmo);
}

void UZSHUDWidget::ShowAmmoWidget()
{
	if (AmmoWidget)
	{
		AmmoWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZSHUDWidget::HideAmmoWidget()
{
	if (AmmoWidget)
	{
		AmmoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
