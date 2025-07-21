// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPBarWidget.h"
#include "Components/ProgressBar.h"
#include "HPBarWidget.h"
#include "Interface/CharacterWidgetInterface.h"

// 언리얼에서 FObjectInitializer를 사용하는 이유:
// UObject 기반 클래스의 생성 타이밍과 메모리 초기화가 일반 C++과 다름
// 초기화 순서를 명확히 하기 위해 Super(ObjectInitializer)를 초기화 리스트에서 반드시 호출하는 구조
UHPBarWidget::UHPBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxHP = -1.f;
}

// 생성자에서는 아직 위젯 트리(디자이너에서 만든 버튼, 프로그래스 바 등)가 구성되지 않았다. 따라서 프로그레스 바 포인터는 NativeConstruct에서 채워준다.
void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBHPBar")));

	// 기본적으로 스탯을 갖는 주체는 캐릭터베이스. 그렇다면 OwningActor를 캐릭터베이스로 캐스팅해야 할까?
	// 그렇게 한다면 이 HP바 위젯은 캐릭터만 가질 수 있게 되어버린다. 의존성이 생기는 것을 방지하기 위해 인터페이스 사용하자
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
