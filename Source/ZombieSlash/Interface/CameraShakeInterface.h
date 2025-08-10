// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/CameraShakeType.h"
#include "CameraShakeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraShakeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESLASH_API ICameraShakeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ShakeCamera(ECameraShakeType ShakeType, float Scale) = 0;
};
