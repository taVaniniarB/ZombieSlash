// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ZSWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UZSWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	// 액터의 BeginPlay 이후 호출
	virtual void InitWidget() override;
};
