#pragma once

#include "CoreMinimal.h"
#include "WeaponState.generated.h"

UENUM(BlueprintType)
enum class EGunState : uint8
{
	Ready = 0,
	Aim,
	Reload
};

UENUM(BlueprintType)
enum class EMeleeState : uint8
{
	Default = 0,
	Parry,
};