// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API AZSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZSPlayerController();

protected:
	virtual void BeginPlay() override;

protected:
	// 생성할 위젯의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UZSHUDWidget> ZSHUDWidgetClass;

	// 생성한 위젯을 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UZSHUDWidget> ZSHUDWidget;
};
