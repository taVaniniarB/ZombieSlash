// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZSWidgetComponent.h"
#include "UI/ZSUserWidget.h"

// WidgetComponent�� BeginPlay ������ ȣ��
void UZSWidgetComponent::InitWidget()
{
	Super::InitWidget(); // ���⼭ ���� �ν��Ͻ��� �����ȴ� (Create Widget)

	// ������ �ν��Ͻ��� ĳ�����Ͽ� �ش� ������ �ڽ��� �����ϴ� ���͸� �˰� �Ѵ�.
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
