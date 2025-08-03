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

	// 이 시점에서 이미 UI에 대한 초기화가 끝난 상태고
	// 에디터에서 배치해둔 위젯들 또한 초기화됐을 것이기 때문에, 에디터에서 설정한 위젯 이름 그대로 사용해서 위젯을 얻어올 수 있다.
	// 위젯 생성 후에도 이것에 접근하기 위해 멤버변수에 넣어준다.

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