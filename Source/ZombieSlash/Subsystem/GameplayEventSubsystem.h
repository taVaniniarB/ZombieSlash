// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Enums/CameraShakeType.h"
#include "GameplayEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCameraShakeRequest, ECameraShakeType, ShakeType, float, Scale);
/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API UGameplayEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCameraShakeRequest OnCameraShakeRequested;

    static UGameplayEventSubsystem* Get(const UObject* WorldContext)
    {
        return UWorld::GetSubsystem<UGameplayEventSubsystem>(WorldContext->GetWorld());
    }
};
