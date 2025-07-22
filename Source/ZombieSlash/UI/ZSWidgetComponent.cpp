// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZSWidgetComponent.h"
#include "UI/ZSUserWidget.h"

// WidgetComponent의 BeginPlay 내에서 호출
void UZSWidgetComponent::InitWidget()
{
	Super::InitWidget(); // 여기서 위젯 인스턴스가 생성된다 (Create Widget)

	// 생성된 인스턴스를 캐스팅하여 해당 위젯도 자신을 소유하는 액터를 알게 한다.
	UZSUserWidget* ZSWidget = Cast<UZSUserWidget>(GetWidget());
	if (ZSWidget)
	{
		ZSWidget->SetOwningActor(GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ZSWidgetComponent: ZSWidget Cast Failed"));
	}
}
