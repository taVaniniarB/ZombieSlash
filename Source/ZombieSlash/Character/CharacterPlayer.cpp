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
#include "Interface/WeaponAnimInterface.h"

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
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));
	if (nullptr != InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
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
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSwitchWeaponRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_SwitchWeapon.IA_SwitchWeapon'"));
	if (nullptr != InputActionSwitchWeaponRef.Object)
	{
		WeaponSwitchAction = InputActionSwitchWeaponRef.Object;
	}
	// Zoom은 블루프린트에서 처리

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

	//Inventory->InitializeWeaponSlots();
	// Weapon Actor Setting
	for (int32 i = 0; i < Inventory->WeaponSlotCount; ++i)
	{
		AWeaponBase* WeaponInst = GetWorld()->SpawnActor<AWeaponBase>(Inventory->WeaponSlots[i].WeaponData->WeaponActorClass);
		Inventory->WeaponSlots[i].WeaponActor = WeaponInst;
		WeaponInst->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponInst->GetSocketName());
		WeaponInst->InitializeWeapon(Inventory->WeaponSlots[i].WeaponData, this);
		WeaponInst->OnUnequip();
	}
	CurWeapon = Inventory->WeaponSlots[0].WeaponActor;
	CurWeapon->OnEquip();

	Stat->SetModifierStat(Inventory->WeaponSlots[0].WeaponData->ModifierStat);
	UpdateWeaponInputMapping(CurWeapon->GetWeaponType());
}

void ACharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Inventory->OnWeaponChanged.AddDynamic(this, &ACharacterPlayer::HandleWeaponChanged);
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

		// Run
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ACharacterBase::SetRunMode);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Attack);

		// Pickup
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::PickupItem);

		// Pickup
		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::UseHealItem);

		// Weapon Switch
		EnhancedInputComponent->BindAction(WeaponSwitchAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::SwitchWeapon);

		// Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Ongoing, this, &ACharacterPlayer::CameraAimZoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ACharacterPlayer::ExitCameraAimZoom);
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

	ExitMontage();
}

void ACharacterPlayer::ExitMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst->IsAnyMontagePlaying())
	{
		FName CurrentSection = AnimInst->Montage_GetCurrentSection();
		FString SectionStr = CurrentSection.ToString();

		if (SectionStr.StartsWith(TEXT("ExitSection")))
		{
			UAnimMontage* Montage = AnimInst->GetCurrentActiveMontage();
			AnimInst->Montage_Stop(0.2f, Montage);
		}
	}
}

void ACharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACharacterPlayer::Attack()
{
	CurWeapon->StartAttack();

	if (EWeaponType::Gun == CurWeapon->GetWeaponType())
	{
		EnterAimState();
	}
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
	//Stat->HealHp(Inventory->GetCurrentHealItem()->HealAmount);
}

void ACharacterPlayer::SetGunState(EGunState GunState, uint8 InIsZooming)
{
	CurGunState = GunState;
	bIsZooming = InIsZooming;
	if (EGunState::Aim == CurGunState)
		bIsAiming = true;
	else
		bIsAiming = false;

	UpdateAnimAimState();
}

void ACharacterPlayer::UpdateAnimAimState()
{
	IWeaponAnimInterface* Anim = Cast<IWeaponAnimInterface>(GetMesh()->GetAnimInstance());
	Anim->SetIsAiming(bIsAiming);
	Anim->SetIsZooming(bIsZooming);
}

void ACharacterPlayer::EnterAimState()
{
	SetGunState(EGunState::Aim, bIsZooming);
	ResetExitAimTimer();
}

void ACharacterPlayer::CameraAimZoom()
{
	SetGunState(EGunState::Aim, true);

	ResetExitAimTimer();
}

void ACharacterPlayer::ExitCameraAimZoom()
{
	SetGunState(EGunState::Aim, false);
}

void ACharacterPlayer::ResetExitAimTimer()
{
	if (GetGunState() == EGunState::Aim)
	{
		GetWorld()->GetTimerManager().ClearTimer(ExitAimTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			ExitAimTimerHandle,
			this,
			&ACharacterPlayer::ExitAimState,
			ExitAimTime,
			false
		);
	}
}

// ExitAim 타이머 종료 시 호출
void ACharacterPlayer::ExitAimState()
{
	SetGunState(EGunState::Ready, false);
}

void ACharacterPlayer::SwitchWeapon(const FInputActionInstance& Value)
{
	const float ScrollValue = Value.GetValue().Get<float>();

	if (FMath::IsNearlyZero(ScrollValue)) return;
	int32 Direction = ScrollValue > 0 ? 1 : -1;

	if (bIsZooming) return;
	if (IsPlayingRootMotion()) return;
	if (!Inventory) return;

	int32 TotalSlots = Inventory->WeaponSlotCount;
	if (TotalSlots == 0) return;

	int32 CurIndex = Inventory->CurWeaponSlotIdx;
	int32 NewIndex = CurIndex;

	// 다음 유효한 무기를 찾음 (비어있는 슬롯은 건너뜀)
	for (int32 i = 0; i < TotalSlots; ++i)
	{
		NewIndex = (NewIndex + Direction + TotalSlots) % TotalSlots;

		if (Inventory->SwitchWeapon(NewIndex))
		{
			break;
		}
	}

	CurWeapon->OnUnequip(); // 이전 무기 장착 해제

	CurWeapon = Inventory->WeaponSlots[NewIndex].WeaponActor;
	Stat->SetModifierStat(Inventory->WeaponSlots[NewIndex].WeaponData->ModifierStat);
	CurWeapon->OnEquip();
	UpdateWeaponInputMapping(CurWeapon->GetWeaponType());
}

// OnWeaponChanged에 바인드됨 (삭제 고려)
void ACharacterPlayer::HandleWeaponChanged(const UWeaponData* Weapon) // 클래스 정보가 오는 게 나은가? 혹은 인덱스
{
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player - HandleWeaponChanged - Weapon was Null"));
		return;
	}

	Stat->SetModifierStat(Weapon->ModifierStat);

	CurWeapon->OnUnequip();

	const int32 Idx = Inventory->CurWeaponSlotIdx;
	CurWeapon = Inventory->WeaponSlots[Idx].WeaponActor;
	CurWeapon->OnEquip();
}

void ACharacterPlayer::Parry()
{
	if (EWeaponType::Melee == CurWeapon->GetWeaponType())
	{
		//SetMeleeState(EMeleeState::Parry);
	}
}

void ACharacterPlayer::UpdateWeaponInputMapping(EWeaponType NewWeaponType)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			if (!Subsystem) return;

			// 모두 제거 (중복 제거 방지)
			if (GunIMC)
			{
				Subsystem->RemoveMappingContext(GunIMC);
			}
			if (MeleeIMC)
			{
				Subsystem->RemoveMappingContext(MeleeIMC);
			}

			// 새 무기에 해당하는 IMC 추가
			switch (NewWeaponType)
			{
			case EWeaponType::Gun:
				if (GunIMC)
				{
					Subsystem->AddMappingContext(GunIMC, 1);
				}
				break;

			case EWeaponType::Melee:
				if (MeleeIMC)
				{
					Subsystem->AddMappingContext(MeleeIMC, 1);
				}
				break;

			default:
				// 무기 없거나 다른 타입일 경우 IMC 없음
				break;
			}
		}
	}
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
					DistScore * 0.60f +
					AngleScore * 0.20f +
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
