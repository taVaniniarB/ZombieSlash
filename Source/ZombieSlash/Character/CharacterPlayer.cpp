// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingcontext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "physics/ZSCollision.h"
#include "Engine/OverlapResult.h"
#include "Item/ItemPickup.h"
#include "Item/InventoryComponent.h"
#include "CharacterStat/CharacterStatComponent.h"

ACharacterPlayer::ACharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPickupRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Pickup.IA_Pickup'"));
	if (nullptr != InputActionPickupRef.Object)
	{
		PickupAction = InputActionPickupRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionHealRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Heal.IA_Heal'"));
	if (nullptr != InputActionHealRef.Object)
	{
		HealAction = InputActionHealRef.Object;
	}

	// Inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("subsystem Not found!!"));

		}
		EnableInput(PlayerController);
	}
}

void ACharacterPlayer::SetDead()
{
	Super::SetDead();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent Called"));
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Look);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Attack);

		// Pickup
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::PickupItem);

		// Pickup
		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::UseHealItem);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ACharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void ACharacterPlayer::PickupItem()
{
	if (OverlappingItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlappingItems was 0"));
	}

	AItemPickup* ClosestItem = nullptr;
	float ClosestDist = FLT_MAX;

	for (AItemPickup* Item : OverlappingItems)
	{
		if (!IsValid(Item)) continue;

		float Dist = FVector::Dist(GetActorLocation(), Item->GetActorLocation());
		if (Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClosestItem = Item;
		}
	}

	if (ClosestItem)
	{
		bool bSuccess = Inventory->AddItem(ClosestItem->ItemData, 1);

		if (bSuccess)
		{
			OverlappingItems.Remove(ClosestItem);
			ClosestItem->Destroy();
		}
	}
}

void ACharacterPlayer::UseHealItem()
{
	//Stat->HealHp(Inventory->GetEquippedHealItem())
}

void ACharacterPlayer::AddOverlappingItem(AItemPickup* InItemData)
{
	OverlappingItems.Add(InItemData);
}

void ACharacterPlayer::RemoveOverlappingItem(AItemPickup* InItemData)
{
	OverlappingItems.Remove(InItemData);
}

AActor* ACharacterPlayer::GetTargetActor()
{
	float DetectRadius = 500.f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, this);

	UWorld* World = GetWorld();
	// �÷��̾ ���� ���̶�� �����̶� ����� �迭�� ����
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, GetActorLocation(), FQuat::Identity, CCHANNEL_ZSACTION, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams
	);

	// ZSAction ä�η� Ư�� �ݰ� �� �浹 ����, ������ ���� AI��Ʈ�ѷ��� ���� ���̶��
	if (bResult)
	{
		float BestScore = -FLT_MAX;
		APawn* BestPawn = nullptr;

		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && !Pawn->GetController()->IsPlayerController())
			{
				// ����ġ �Űܼ� ���� ������ ���� ���� Ÿ������ �����Ѵ�.

				// ------ ���׸� ��� ------
				const float Dist = FVector::Distance(GetActorLocation(), Pawn->GetActorLocation());
				const float DistScore = 1.f - FMath::Clamp(Dist / DetectRadius, 0.f, 1.f);

				const FVector Dir = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				const float AngleScore = FMath::Clamp((FVector::DotProduct(GetActorForwardVector(), Dir) * 0.5f) + 0.5f, 0.f, 1.f);


				const bool  PrevTarget = (Pawn == AttackTarget);
				const float StickyScore = PrevTarget ? 1.f : 0.f;

				// ------ ������ ------
				const float Score =
					DistScore * 0.50f +
					AngleScore * 0.30f +
					StickyScore * 0.20f;

				if (Score > BestScore)
				{
					BestScore = Score;
					BestPawn = Pawn;
				}
			}

			AttackTarget = BestPawn;

			// ���� �� ���� �ֺ����� ���� �ݰ��� ��Ÿ���� ��� ��ü�� �׸���
			DrawDebugSphere(World, GetActorLocation(), DetectRadius, 16, FColor::Green, false, 0.2f);
			// ������ �� �ֺ����� ���� �׸���
			DrawDebugPoint(World, BestPawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
			// ������ �� - �÷��̾� �� ���� �ߴ´�
			DrawDebugLine(World, GetActorLocation(), BestPawn->GetActorLocation(), FColor::Green, false, 0.27f);

		}
	}
	else
	{
		AttackTarget = nullptr;

		DrawDebugSphere(World, GetActorLocation(), DetectRadius, 16, FColor::Red, false, 0.2f);
	}
	return AttackTarget;
}
