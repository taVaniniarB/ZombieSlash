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
#include "Inventory/InventoryComponent.h"
#include "Item/ItemData.h"
#include "CharacterStat/CharacterStatComponent.h"
#include "UI/ZSHUDWidget.h"
#include "Interface/WeaponAnimInterface.h"
#include "Inventory/QuickSlot.h"
#include "Player/ZSPlayerController.h"
#include "Camera/CameraControlComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Weapon/PlayerWeaponManagerComponent.h"

ACharacterPlayer::ACharacterPlayer()
{
	SetCharacterID(TEXT("Player_Default"));

	// Inventory
	QuickSlot = CreateDefaultSubobject<UQuickSlot>(TEXT("QuickSlot"));
	QuickSlot->SetInventorySize(2);

	WeaponManager = CreateDefaultSubobject<UPlayerWeaponManagerComponent>(TEXT("WeaponManager"));

	// Interaction
	Interaction = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 45.f, 70.f);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 22.f));
	CameraBoom->bUsePawnControlRotation = true;
	CameraController = CreateDefaultSubobject<UCameraControlComponent>(TEXT("CameraController"));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
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

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> CombatMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Combat.IMC_Combat'"));
	if (nullptr != CombatMappingContextRef.Object)
	{
		CombatIMC = CombatMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/CombatAction/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSwitchWeaponRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/CombatAction/IA_SwitchWeapon.IA_SwitchWeapon'"));
	if (nullptr != InputActionSwitchWeaponRef.Object)
	{
		WeaponSwitchAction = InputActionSwitchWeaponRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InteractionMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Interaction.IMC_Interaction'"));
	if (nullptr != InteractionMappingContextRef.Object)
	{
		InteractionIMC = InteractionMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPickupRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InteractionAction/IA_Pickup.IA_Pickup'"));
	if (nullptr != InputActionPickupRef.Object)
	{
		InteractionAction = InputActionPickupRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionHealRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InteractionAction/IA_Heal.IA_Heal'"));
	if (nullptr != InputActionHealRef.Object)
	{
		QuickSlotX = InputActionHealRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> GunMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Gun.IMC_Gun'"));
	if (nullptr != GunMappingContextRef.Object)
	{
		GunIMC = GunMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ZoomActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/GunAction/IA_Zoom.IA_Zoom'"));
	if (nullptr != ZoomActionRef.Object)
	{
		ZoomAction = ZoomActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/GunAction/IA_Reload.IA_Reload'"));
	if (nullptr != ReloadActionRef.Object)
	{
		ReloadAction = ReloadActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MeleeMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Melee.IMC_Melee'"));
	if (nullptr != MeleeMappingContextRef.Object)
	{
		MeleeIMC = MeleeMappingContextRef.Object;
	}
}

void ACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Input Setting
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(InteractionIMC, 0);
			Subsystem->AddMappingContext(CombatIMC, 0);
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

		// Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Interact);

		// QuickSlot(X)
		EnhancedInputComponent->BindAction(QuickSlotX, ETriggerEvent::Triggered, this, &ACharacterPlayer::UseQuickSlotX);

		// Weapon Switch
		EnhancedInputComponent->BindAction(WeaponSwitchAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::SwitchWeapon);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Reload);

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

	// 사용자의 즉각적인 반응성 확보 위해 이동 입력 시 항상 ExitMontage 호출
	ExitMontage();
}

void ACharacterPlayer::ExitMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

	// 애니메이션 몽타주가 재생 중이 아닐 경우 함수 종료
	if (!AnimInst 
		|| !AnimInst->IsAnyMontagePlaying()) return;

	// 현재 재생 중인 섹션 이름을 문자열로 변환
	FName CurrentSection = AnimInst->Montage_GetCurrentSection();
	FString SectionStr = CurrentSection.ToString();

	// 섹션 이름이 "ExitSection"으로 시작하는지 확인
	if (SectionStr.StartsWith(ExitSectionPrefix))
	{
		UAnimMontage* Montage = AnimInst->GetCurrentActiveMontage();

		if (Montage)
		{
			// 몽타주를 부드럽게 중지
			AnimInst->Montage_Stop(ExitBlendOutTime, Montage);
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
	Super::Attack();

	WeaponManager->Attack();
}

bool ACharacterPlayer::PickupItem(FPrimaryAssetId ItemID, EItemType ItemType, int32 Quantity)
{
	bool QuickSlotSuccess = false;
	if (ItemType == EItemType::Usable)
	{
		QuickSlotSuccess = QuickSlot->AddItem(ItemID, Quantity);
	}

	bool MainInventorySuccess = false;
	if (!QuickSlotSuccess)
	{
		MainInventorySuccess = Inventory->AddItem(ItemID, Quantity);
	}

	if (QuickSlotSuccess || MainInventorySuccess)
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup Item Fail"));
	}
	return false;
}

void ACharacterPlayer::Interact()
{
	Interaction->TryInteract();
}

bool ACharacterPlayer::HasItem_Implementation(FPrimaryAssetId ItemID)
{
	return Inventory->HasItem(ItemID);
}

void ACharacterPlayer::UseQuickSlotX()
{
	QuickSlot->UseItemByIndex(0, 1);
}

void ACharacterPlayer::SetGunState(EGunState NewGunState, uint8 InIsZooming, bool PlayMontage)
{
	if (CurGunState != NewGunState && PlayMontage && !(CurGunState == EGunState::Reload))
	{
		K2_OnGunStateChanged(NewGunState);
	}

	CurGunState = NewGunState;
	bIsZooming = InIsZooming;

	bIsAiming = (CurGunState == EGunState::Aim);
	bIsReloading = (CurGunState == EGunState::Reload);

	UpdateAnimAimState();
}

void ACharacterPlayer::UpdateAnimAimState()
{
	IWeaponAnimInterface* Anim = Cast<IWeaponAnimInterface>(GetMesh()->GetAnimInstance());
	Anim->SetIsAiming(bIsAiming);
	Anim->SetIsReloading(bIsReloading);
	Anim->SetIsZooming(bIsZooming);
}

void ACharacterPlayer::EnterAimState()
{
	SetGunState(EGunState::Aim, bIsZooming, true);
	ResetExitAimTimer();
}

void ACharacterPlayer::CameraAimZoom()
{
	SetGunState(EGunState::Aim, true, true);
	ResetExitAimTimer();
}

void ACharacterPlayer::ExitCameraAimZoom()
{
	SetGunState(EGunState::Aim, false, true);
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
	SetGunState(EGunState::Ready, false, true);
}

void ACharacterPlayer::Reload()
{
	if (WeaponManager->Reload())
	{
		ActiveCombatAction(false);
		SetGunState(EGunState::Reload, bIsZooming, false);
	}
}

void ACharacterPlayer::EndReload()
{
	ActiveCombatAction(true);
	SetGunState(EGunState::Aim, bIsZooming, false);
	ResetExitAimTimer();
}

void ACharacterPlayer::SwitchWeapon(const FInputActionInstance& Value)
{
	WeaponManager->SwitchWeapon(Value);
	UpdateMovementSpeed();
}


void ACharacterPlayer::HandleAmmoChanged(int32 NewAmmo, int32 MaxAmmo)
{
	// 플레이어 컨트롤러를 통해 HUD에 알림
	AZSPlayerController* PC = Cast<AZSPlayerController>(GetController());
	if (PC)
	{
		UZSHUDWidget* HUDWidget = PC->GetHUDWidget();
		if (HUDWidget)
		{
			HUDWidget->UpdateAmmo(NewAmmo, MaxAmmo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDWidget Null"));
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("PC Null"));
}

void ACharacterPlayer::Parry()
{
	//if (EWeaponType::Melee == Weapons[CurWeaponIndex]->GetWeaponType())
	//{
	//	SetMeleeState(EMeleeState::Parry);
	//}
}

void ACharacterPlayer::UpdateWeaponIMC(EWeaponType NewWeaponType)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			if (!Subsystem) return;

			// 기존 IMC 모두 제거
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
				break;
			}
		}
	}
}

void ACharacterPlayer::ActiveCombatAction(bool bActive)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			if (!Subsystem) return;
			if (!CombatIMC)
			{
				UE_LOG(LogTemp, Warning, TEXT("CombatIMC is Null"));
				return;
			}

			if (bActive)
			{
				Subsystem->AddMappingContext(CombatIMC, 1);
			}
			else
			{
				Subsystem->RemoveMappingContext(CombatIMC);
			}
		}
	}
}

AActor* ACharacterPlayer::GetClosestInteractable()
{
	if (Interaction)
	{
		return Interaction->ClosestInteractable;
	}
	return nullptr;
}

void ACharacterPlayer::AddOverlappingInteractable_Implementation(AActor* InInteractable)
{
	Interaction->AddOverlappingInteractable(InInteractable);
}

void ACharacterPlayer::RemoveOverlappingInteractable_Implementation(AActor* InInteractable)
{
	Interaction->RemoveOverlappingInteractable(InInteractable);
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

void ACharacterPlayer::BindWeaponEquippedEvent(UZSHUDWidget* InHUDWidget)
{
	check(WeaponManager);
	check(InHUDWidget);

	WeaponManager->OnWeaponEquipped.AddDynamic(InHUDWidget, &UZSHUDWidget::HandleWeaponEquipped);
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
#if ENABLE_DRAW_DEBUG
			// 감지 시 본인 주변으로 감지 반경을 나타내는 녹색 구체를 그린다
			//DrawDebugSphere(World, GetActorLocation(), DetectRadius, 16, FColor::Green, false, 0.2f);
			// 감지된 적 주변으로 점을 그린다
			//DrawDebugPoint(World, BestPawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
			// 감지된 적 - 플레이어 간 선을 긋는다
			//DrawDebugLine(World, GetActorLocation(), BestPawn->GetActorLocation(), FColor::Green, false, 0.27f);
#endif
		}
	}
	else
	{
		AttackTarget = nullptr;
#if ENABLE_DRAW_DEBUG
		//DrawDebugSphere(World, GetActorLocation(), DetectRadius, 16, FColor::Red, false, 0.2f);
#endif
	}
	return AttackTarget;
}