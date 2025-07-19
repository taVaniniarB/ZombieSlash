// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZSUserWidget.generated.h"

// UZSUserWidget을 상속받는 위젯은 GetOwner를 통해 자신을 소유하는 액터를 알 수 있다.
// 이를 통해 자신의 주인 액터의 델리게이트에 자신의 함수를 등록할 수 있다.

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UZSUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
