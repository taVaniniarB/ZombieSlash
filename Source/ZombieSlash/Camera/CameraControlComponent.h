// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CameraShakeType.h"
#include "CameraControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESLASH_API UCameraControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UCameraControlComponent();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void PlayCameraShake(ECameraShakeType ShakeType, float Scale);

private:
    UPROPERTY()
    UDataTable* CameraShakeDataTable;
};
