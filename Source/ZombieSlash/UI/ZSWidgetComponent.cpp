// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZSWidgetComponent.h"
#include "UI/ZSUserWidget.h"

void UZSWidgetComponent::InitWidget()
{
	Super::InitWidget(); // ���⼭ ���� �ν��Ͻ��� �����ȴ�

	// ������ �ν��Ͻ��� ĳ�����Ͽ� �ش� ������ �ڽ��� �����ϴ� ���͸� �˰� �Ѵ�.
	UZSUserWidget* UZWidget = Cast<UZSUserWidget>(GetWidget());
	if (UZWidget)
	{
		UZWidget->SetOwningActor(GetOwner());
	}
}
