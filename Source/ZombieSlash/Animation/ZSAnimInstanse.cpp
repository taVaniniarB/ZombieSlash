// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ZSAnimInstanse.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UZSAnimInstanse::UZSAnimInstanse()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

	bIsZooming = false;
	bIsAiming = false;
	bIsShooting = false;
}

void UZSAnimInstanse::Shoot()
{
	bIsShooting = true;
	GetWorld()->GetTimerManager().SetTimer(
		ShootAnimTimer,
		this,
		&UZSAnimInstanse::ResetIsShooting,
		0.1f,
		false);
}

void UZSAnimInstanse::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UZSAnimInstanse::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bShouldMove = (GroundSpeed > 0) && (FVector::ZeroVector != Movement->GetCurrentAcceleration());
	}
}
