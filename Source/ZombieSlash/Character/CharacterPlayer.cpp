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
#include "Item/ItemData.h"
#include "Item/WeaponData.h"
#include "Item/GunData.h"
#include "Weapon/WeaponBase.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "UI/ZSHUDWidget.h"

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

	// Weapon Component
	//WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	
	// Inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

}

void ACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateClosestItem();
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Input Setting
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		EnableInput(PlayerController);
	}

	// Weapon Actor Setting
	//CurWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	CurWeapon = GetWorld()->SpawnActor<AWeaponBase>(Inventory->WeaponSlots[0]->WeaponActorClass);
	CurWeapon->InitializeWeapon(Inventory->WeaponSlots[0], this);
	CurWeapon->OnEquip();
	Stat->SetModifierStat(Inventory->WeaponSlots[0]->ModifierStat);
	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurWeapon->SocketName);
}

void ACharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Inventory->OnWeaponChanged.AddDynamic(this, &ACharacterPlayer::HandleWeaponChanged);
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
	//if (CurWeapon->WeaponType == EWeaponType::Gun)
	//{
	//	//Fire();
	//	return;
	//}
	//else
	//{
	//
	// CurWeapon->StartAttack();
	ProcessComboCommand();
}

void ACharacterPlayer::PickupItem()
{
	if (OverlappingItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlappingItems was 0"));
	}

	if (ClosestItem)
	{
		bool bSuccess = Inventory->AddItem(ClosestItem->ItemData, ClosestItem->Quantity);

		for (int i = 0; i < Inventory->MaxSlotCount; ++i)
		{
			if (IsValid(Inventory->Items[i].ItemData))
				UE_LOG(LogTemp, Warning, TEXT("%s, %d개, 슬롯: %d"), *Inventory->Items[i].ItemData->Name, Inventory->Items[i].Quantity, i);
		}
			
		if (bSuccess)
		{
			OverlappingItems.Remove(ClosestItem);
			ClosestItem->Destroy();
		}
	}
}

void ACharacterPlayer::UseHealItem()
{
	if (Inventory->GetItemCountByType(EItemType::HealItem) <= 0) return;
	
	Inventory->UseItem(Inventory->GetCurHealItemID());
	// Stat->HealHp(Inventory->GetCurrentHealItem()->HealAmount);
}

void ACharacterPlayer::HandleWeaponChanged(const UWeaponData* Weapon) // 클래스 정보가 오는 게 나은가? 혹은 인덱스
{
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player - HandleWeaponChanged - Weapon was Null"));
		return;
	}
	
	//WeaponMesh->SetStaticMesh(Weapon->WeaponMesh); //액터 스폰 방식으로 바꾸면 필요없을듯
	Stat->SetModifierStat(Weapon->ModifierStat);

	CurWeapon->OnUnequip();

	// 액터가 스스로 처리
	//	// 소켓과 본 위치 조정
	//	// 에임 카메라 비활성화
	//	// UI (크로스헤어, 총알)

	// 슬롯인덱스 수정
	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(Inventory->WeaponSlots[0]->WeaponActorClass);
	NewWeapon->InitializeWeapon(Inventory->WeaponSlots[0], this);
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, NewWeapon->SocketName);
	NewWeapon->OnEquip();
}

void ACharacterPlayer::AddOverlappingItem(AItemPickup* InItemData)
{
	OverlappingItems.Add(InItemData);
	//UE_LOG(LogTemp, Warning, TEXT("Overlapping Items %d"), OverlappingItems.Num());
}

void ACharacterPlayer::RemoveOverlappingItem(AItemPickup* InItemData)
{
	OverlappingItems.Remove(InItemData);
	//UE_LOG(LogTemp, Warning, TEXT("Overlapping Items %d"), OverlappingItems.Num());
}

void ACharacterPlayer::UpdateClosestItem()
{
	ClosestItem = nullptr;
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
}

void ACharacterPlayer::SetupHUDWidget(UZSHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHPBar(Stat->GetCurHP());

		Stat->OnHPChanged.AddUObject(InHUDWidget, &UZSHUDWidget::UpdateHPBar);
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UZSHUDWidget::UpdateStat);
	}
}

AActor* ACharacterPlayer::GetTargetActor()
{
	float DetectRadius = 500.f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, this);

	UWorld* World = GetWorld();
	// 플레이어가 여러 명이라는 가정이라서 결과를 배열로 받음
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, GetActorLocation(), FQuat::Identity, CCHANNEL_ZSACTION, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams
	);

	// ZSAction 채널로 특정 반경 내 충돌 감지, 감지된 것이 AI컨트롤러를 지닌 적이라면
	if (bResult)
	{
		float BestScore = -FLT_MAX;
		APawn* BestPawn = nullptr;

		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && !Pawn->GetController()->IsPlayerController())
			{
				// 가중치 매겨서 가장 점수가 높은 것을 타겟으로 선정한다.

				// ------ 평가항목 계산 ------
				const float Dist = FVector::Distance(GetActorLocation(), Pawn->GetActorLocation());
				const float DistScore = 1.f - FMath::Clamp(Dist / DetectRadius, 0.f, 1.f);

				const FVector Dir = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				const float AngleScore = FMath::Clamp((FVector::DotProduct(GetActorForwardVector(), Dir) * 0.5f) + 0.5f, 0.f, 1.f);


				const bool  PrevTarget = (Pawn == AttackTarget);
				const float StickyScore = PrevTarget ? 1.f : 0.f;

				// ------ 가중합 ------
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

			// 감지 시 본인 주변으로 감지 반경을 나타내는 녹색 구체를 그린다
			DrawDebugSphere(World, GetActorLocation(), DetectRadius, 16, FColor::Green, false, 0.2f);
			// 감지된 적 주변으로 점을 그린다
			DrawDebugPoint(World, BestPawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
			// 감지된 적 - 플레이어 간 선을 긋는다
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
