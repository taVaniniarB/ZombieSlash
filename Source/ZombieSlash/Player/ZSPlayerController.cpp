// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZSPlayerController.h"
#include "ZSPlayerController.h"
#include "UI/ZSHUDWidget.h"
#include "Character/CharacterPlayer.h"

AZSPlayerController::AZSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UZSHUDWidget> ZSHUDWidgetRef(TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
	if (ZSHUDWidgetRef.Class)
	{
		ZSHUDWidgetClass = ZSHUDWidgetRef.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ZSHUDWidgetRef.Class was Null"));
	}
}


void AZSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	/*ZSHUDWidget = CreateWidget<UZSHUDWidget>(this, ZSHUDWidgetClass);
	if (ZSHUDWidget)
	{
		ZSHUDWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ZSHUDWidget was Null"));
	}

	if (InteractWidgetClass)
	{
		InteractWidget = CreateWidget<UUserWidget>(this, InteractWidgetClass);
		if (InteractWidget)
		{
			InteractWidget->AddToViewport();
			InteractWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/
}

void AZSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CachedPlayerPawn = Cast<ACharacterPlayer>(InPawn);
}
