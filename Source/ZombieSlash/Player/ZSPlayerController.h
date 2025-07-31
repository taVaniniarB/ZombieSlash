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
	// HUD ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UZSHUDWidget> ZSHUDWidgetClass;

	// HUD ���� �ν��Ͻ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UZSHUDWidget> ZSHUDWidget;

	// ��ȣ�ۿ� �޽��� ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	TSubclassOf<class UUserWidget> InteractWidgetClass;

	// ��ȣ�ۿ� �޽��� ���� �ν��Ͻ�
	UPROPERTY()
	TObjectPtr<class UUserWidget> InteractWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class ACharacterPlayer> CachedPlayerPawn;

	virtual void OnPossess(APawn* InPawn) override;

	// Inventory Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputMappingContext> UIInteractionIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;
};
