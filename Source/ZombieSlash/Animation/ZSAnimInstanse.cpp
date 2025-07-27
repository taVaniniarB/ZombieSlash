// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ZSAnimInstanse.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UZSAnimInstanse::UZSAnimInstanse()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
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
	}
	if (bIsIdle)
	{
		//UE_LOG(LogTemp, Warning, TEXT("IsIdle"));
	}
}
